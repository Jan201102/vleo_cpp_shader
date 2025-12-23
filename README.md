# VLEO Cpp Shader

A C++ shader project utilizing modern graphics libraries.

---

## Binary Shader

### Dependencies

| Library | Header | Include Path | Library Path |
|---------|--------|---------------|--------------|
| **GLEW** | `GL/glew.h` | `$(SolutionDir)Dependencies\GLEW\include` | — |
| **OpenGL** | — | — | — |
| **GLM** | — | `src/vendor/glm` | — |
| **GLFW** | `GLFW/glfw3.h` | `$(SolutionDir)Dependencies\GLFW\include` | — |

---

## vleo-cpp-shader

### Dependencies

| Library | Header | Include Path | Library Path | Link |
|---------|--------|---------------|--------------|------|
| **GLEW** | `GL/glew.h` | `$(SolutionDir)Dependencies\GLEW\include` | `$(SolutionDir)Dependencies\GLEW\lib\Release\x64` | `glew32s.lib` |
| **OpenGL** | — | — | — | `opengl32.lib` |
| **GLM** | — | `src/vendor/glm` | — | — |
| **GLFW** | `GLFW/glfw3.h` | `$(SolutionDir)Dependencies\GLFW\include` | `$(SolutionDir)Dependencies\GLFW\lib-vc2022` | `glfw3.lib` |