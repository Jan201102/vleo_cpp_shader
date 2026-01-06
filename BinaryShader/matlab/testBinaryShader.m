%% add lib to path
disp("Adding BinaryShader to path...");
doc = matlab.desktop.editor.getActive
doc.Filename
folder = fileparts(doc.Filename)
addpath(fullfile(folder,"/BinaryShader"));
disp("Added BinaryShader to path.");
%% include teteagedron geometry
run(fullfile(folder,"two_tetraedrons.m"))

%% test call
% 1. Define data
% Number of vertices and triangles
numTriangles = length(triangleIDs)/3;

shadedData = false(1,numTriangles);

shadedArg = clibConvertArray("clib.BinaryShader.Bool",shadedData);
verticesArg = clibConvertArray("clib.BinaryShader.Float",vertices);
triangleIDsArg = clibConvertArray("clib.BinaryShader.UnsignedInt",triangleIDs);

% 4. Call the function
try
    % Pass the clib objects and the uint64 lengths
    disp("Calling BinaryRenderer...");
    clib.BinaryShader.BinaryRenderer(verticesArg,triangleIDsArg,shadedArg, 1.0, 0.0, 0.0);

    % 5. Convert back to MATLAB to see the result
    finalShades = logical(shadedArg);
    disp('Test completed. Results:');
    disp(finalShades);
catch ME
    fprintf('Error: %s\n', ME.message);
end