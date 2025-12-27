%% add lib to path
doc = matlab.desktop.editor.getActive
doc.Filename
folder = fileparts(doc.Filename)
addpath(fullfile(folder,"/BinaryShader"));

%% include teteagedron geometry
run(fullfile(folder,"tetraeder.m"))

%% test call
% 1. Define data
shadedData = [false,false,false,false];

shadedArg = clibConvertArray("clib.BinaryShader.Bool",shadedData);
verticesArg = clibConvertArray("clib.BinaryShader.Float",vertices);
triangleIDsArg = clibConvertArray("clib.BinaryShader.UnsignedInt",triangleIDs);

% 4. Call the function
try
    % Pass the clib objects and the uint64 lengths
    clib.BinaryShader.BinaryRenderer(verticesArg,triangleIDsArg,shadedArg, 0.0, 0.0, 1.0);

    % 5. Convert back to MATLAB to see the result
    finalShades = logical(shadedArg);
    disp('Test completed. Results:');
    disp(finalShades);
catch ME
    fprintf('Error: %s\n', ME.message);
end