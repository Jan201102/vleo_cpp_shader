%setup comiler
clear;
compilerPath = char(string(fullfile(matlabroot,'\bin\win64\mexopts\msvcpp2022.xml')))
mex(['-setup:' compilerPath], 'C++')
%%
%fullfilePath = mfilename('fullpath');
%Note: mfilename returns empty when run from the Command Window or from certain callbacks.
%folder = fileparts(fullfilePath);
% solutionDir = fullfile(folder,'../')
%%
doc = matlab.desktop.editor.getActive
doc.Filename
folder = fileparts(doc.Filename)
solutionDir = char(java.io.File(fullfile(folder,"../")).getCanonicalPath())
%%
mainlib = fullfile(solutionDir,"x64/Release/BinaryShader.lib")
mainHeader =  fullfile(solutionDir,"BinaryShader/BinaryShader.h")
glewLib = fullfile(solutionDir,"Dependencies/GLEW/lib/Release/x64/glew32s.lib")
glfwLib = fullfile(solutionDir,"Dependencies/GLFW/lib-vc2022/glfw3.lib")
glewInclude = fullfile(solutionDir,"Dependencies/GLEW/include")
glfwInclude = fullfile(solutionDir,"Dependencies/GLFW/include")
glmInclude = fullfile(solutionDir,"BinaryShader/src/vendor/glm")

% Find Windows SDK libraries
% Try common Windows SDK locations
windowsKitBase = 'C:\Program Files (x86)\Windows Kits\10\Lib';
if isfolder(windowsKitBase)
    % Find the latest SDK version
    sdkVersions = dir(windowsKitBase);
    sdkVersions = sdkVersions([sdkVersions.isdir]);
    sdkVersions = sdkVersions(~ismember({sdkVersions.name}, {'.', '..'}));
    if ~isempty(sdkVersions)
        % Use the last (latest) version
        sdkVersion = sdkVersions(end).name;
        windowsLibPath = fullfile(windowsKitBase, sdkVersion, 'um\x64');
        
        opengl32Lib = fullfile(windowsLibPath, 'opengl32.lib');
        user32Lib = fullfile(windowsLibPath, 'user32.lib');
        gdi32Lib = fullfile(windowsLibPath, 'gdi32.lib');
        shell32Lib = fullfile(windowsLibPath, 'shell32.lib');
        
        % Verify all libraries exist
        if ~isfile(opengl32Lib), error('opengl32.lib not found'); end
        if ~isfile(user32Lib), error('user32.lib not found'); end
        if ~isfile(gdi32Lib), error('gdi32.lib not found'); end
        if ~isfile(shell32Lib), error('shell32.lib not found'); end
        
        allLibs = [mainlib, glewLib, glfwLib, opengl32Lib, user32Lib, gdi32Lib, shell32Lib];
        
        % Display libraries for verification
        disp('Libraries to be included:');
        disp(allLibs);
    else
        error('Could not find Windows SDK version');
    end
else
    error('Windows SDK not found at expected location');
end

%%
clibgen.generateLibraryDefinition(mainHeader,...
    "Libraries", allLibs,...
    "IncludePath",[glmInclude,glewInclude,glfwInclude],...
    "DefinedMacros", "GLEW_STATIC",...
    "InterfaceName", "BinaryShader",...
    "Verbose", true,...
    'OverwriteExistingDefinitionFiles',true);
%%
summary(defineBinaryShader)
%%
build(defineBinaryShader);



