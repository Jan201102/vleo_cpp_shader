%% About defineBinaryShader.m
% This file defines the MATLAB interface to the library |BinaryShader|.
%
% Commented sections represent C++ functionality that MATLAB cannot automatically define. To include
% functionality, uncomment a section and provide values for <SHAPE>, <DIRECTION>, etc. For more
% information, see helpview('matlab','cpp_define_interface') to "Define MATLAB Interface for C++ Library".



%% Setup
% Do not edit this setup section.
function libDef = defineBinaryShader()
libDef = clibgen.LibraryDefinition("BinaryShaderData.xml");

%% OutputFolder and Libraries 
libDef.OutputFolder = "C:\Users\Jan_L\OneDrive\Dokumente\Arbeit\HIWI\IFR\vleo_cpp_shader\BinaryShader\matlab";
libDef.Libraries = [ "C:\Users\Jan_L\OneDrive\Dokumente\Arbeit\HIWI\IFR\vleo_cpp_shader\x64\Release\BinaryShader.lib" "C:\Users\Jan_L\OneDrive\Dokumente\Arbeit\HIWI\IFR\vleo_cpp_shader\Dependencies\GLEW\lib\Release\x64\glew32s.lib" "C:\Users\Jan_L\OneDrive\Dokumente\Arbeit\HIWI\IFR\vleo_cpp_shader\Dependencies\GLFW\lib-vc2022\glfw3.lib" "C:\Program Files (x86)\Windows Kits\10\Lib\10.0.26100.0\um\x64\opengl32.lib" "C:\Program Files (x86)\Windows Kits\10\Lib\10.0.26100.0\um\x64\user32.lib" "C:\Program Files (x86)\Windows Kits\10\Lib\10.0.26100.0\um\x64\gdi32.lib" "C:\Program Files (x86)\Windows Kits\10\Lib\10.0.26100.0\um\x64\shell32.lib" ];

%% C++ function |BinaryRenderer| with MATLAB name |clib.BinaryShader.BinaryRenderer|
% C++ Signature: int BinaryRenderer(float [] vertecies,size_t lenVertices,unsigned int [] triangleIDs,size_t lenTriangleIDs,bool [] isTriangleVisible,size_t lenIsTriangleVisible,float windX,float windY,float windZ)

BinaryRendererDefinition = addFunction(libDef, ...
   "int BinaryRenderer(float [] vertecies,size_t lenVertices,unsigned int [] triangleIDs,size_t lenTriangleIDs,bool [] isTriangleVisible,size_t lenIsTriangleVisible,float windX,float windY,float windZ)", ...
   "MATLABName", "clib.BinaryShader.BinaryRenderer", ...
   "Description", "clib.BinaryShader.BinaryRenderer Representation of C++ function BinaryRenderer."); % Modify help description values as needed.
defineArgument(BinaryRendererDefinition, "vertecies", "clib.array.BinaryShader.Float", "input", "lenVertices"); % <MLTYPE> can be "clib.array.BinaryShader.Float", or "single"
defineArgument(BinaryRendererDefinition, "lenVertices", "uint64");
defineArgument(BinaryRendererDefinition, "triangleIDs", "clib.array.BinaryShader.UnsignedInt", "input", "lenTriangleIDs"); % <MLTYPE> can be "clib.array.BinaryShader.UnsignedInt", or "uint32"
defineArgument(BinaryRendererDefinition, "lenTriangleIDs", "uint64");
defineArgument(BinaryRendererDefinition, "isTriangleVisible", "clib.array.BinaryShader.Bool", "input", "lenIsTriangleVisible"); % <MLTYPE> can be "clib.array.BinaryShader.Bool", or "logical"
defineArgument(BinaryRendererDefinition, "lenIsTriangleVisible", "uint64");
defineArgument(BinaryRendererDefinition, "windX", "single");
defineArgument(BinaryRendererDefinition, "windY", "single");
defineArgument(BinaryRendererDefinition, "windZ", "single");
defineOutput(BinaryRendererDefinition, "RetVal", "int32");
validate(BinaryRendererDefinition);

%% Validate the library definition
validate(libDef);

end
