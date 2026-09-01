#include <OpenGL/Graphics/GraphicsEngine.h>
#include <windows.h>
#include <glad/glad.h>
#include <glad/glad_wgl.h>
#include <assert.h>
#include <stdexcept>
#include <OpenGL/Graphics/VertexArrayObject.h>
#include <OpenGL/Graphics/ShaderProgram.h>
#include <OpenGL/Graphics/UniformBuffer.h>

GraphicsEngine::GraphicsEngine() {
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpszClassName = (LPCWSTR)"OpenGLdummyWindow";
    wc.lpfnWndProc = DefWindowProc;
    wc.style = CS_OWNDC;

    auto classID = RegisterClassEx(&wc);
    assert(classID);

    auto dummyWindow = CreateWindowEx(0, MAKEINTATOM(classID), (LPCWSTR)"", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, GetModuleHandle(NULL), NULL);

    assert(dummyWindow);

    auto dummyDC = GetDC(dummyWindow);

    PIXELFORMATDESCRIPTOR pixelFormatDesc = {};
    pixelFormatDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pixelFormatDesc.nVersion = 1;
    pixelFormatDesc.iPixelType = PFD_TYPE_RGBA;
    pixelFormatDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pixelFormatDesc.cColorBits = 32;
    pixelFormatDesc.cAlphaBits = 8;
    pixelFormatDesc.cDepthBits = 24;
    pixelFormatDesc.cStencilBits = 8;
    pixelFormatDesc.iLayerType = PFD_MAIN_PLANE;

    auto pixelFormat = ChoosePixelFormat(dummyDC, &pixelFormatDesc);
    SetPixelFormat(dummyDC, pixelFormat, &pixelFormatDesc);

    auto dummyContext = wglCreateContext(dummyDC);
    assert(dummyContext);

    wglMakeCurrent(dummyDC, dummyContext);

    if (!gladLoadWGL(dummyDC)) {
        DaEngineB_ERROR("ERROR! GraphicsEngine: gladLoadWGL failed!");
    }

    if (!gladLoadGL()) {
        DaEngineB_ERROR("ERROR! GraphicsEngine: gladLoadGL failed!");
    }

    wglMakeCurrent(dummyDC, 0);
    wglDeleteContext(dummyContext);
    ReleaseDC(dummyWindow, dummyDC);
    DestroyWindow(dummyWindow);
}

GraphicsEngine::~GraphicsEngine() {

}

void GraphicsEngine::enableDepth() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

void GraphicsEngine::clear(const Vec4 &color) {
    glClearColor(color.x, color.y, color.z, color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GraphicsEngine::setViewPort(const Rect &size) {
    glViewport(size.left, size.top, size.width, size.height);
}

VertexArrayObjectPtr GraphicsEngine::createVertexArrayObject(const VertexBufferDesc &vbDesc) {
    return std::make_shared<VertexArrayObject>(vbDesc);
}

void GraphicsEngine::setVertexArrayObject(const VertexArrayObjectPtr &vao) {
    glBindVertexArray(vao->getID());
}

void GraphicsEngine::drawTriangles(const TriangleType &triangleType, unsigned int vertexCount, unsigned int offset) {
    auto glTriangleType = GL_TRIANGLES;
    switch (triangleType) {
        case TriangleType::TriangleList: {
            glTriangleType = GL_TRIANGLES; break;
        }
        case TriangleType::TriangleStrip: {
            glTriangleType = GL_TRIANGLE_STRIP; break;
        }
    }

    glDrawArrays(glTriangleType, offset, vertexCount);
}

ShaderProgramPtr GraphicsEngine::createShaderProgram(const ShaderProgramDesc &desc) {
    return std::make_shared<ShaderProgram>(desc);
}

void GraphicsEngine::setShaderProgram(const ShaderProgramPtr &program) {
    glUseProgram(program->getID());
}

UniformBufferPtr GraphicsEngine::createUniformBuffer(const UniformBufferDesc &desc) {
    return std::make_shared<UniformBuffer>(desc);
}

void GraphicsEngine::setUniformBuffer(UniformBufferPtr &buffer, unsigned int slot) {
    glBindBufferBase(GL_UNIFORM_BUFFER, slot, buffer->getID());
}

VertexArrayObjectPtr GraphicsEngine::createVertexArrayObject(const VertexBufferDesc &vbDesc, const IndexBufferDesc &ibDesc) {
    return std::make_shared<VertexArrayObject>(vbDesc, ibDesc);
}

void GraphicsEngine::drawIndexedTriangles(const TriangleType &triangleType, unsigned int indicesCount) {
    auto glTriangleType = GL_TRIANGLES;
    switch (triangleType) {
        case TriangleType::TriangleList: {
            glTriangleType = GL_TRIANGLES; break;
        }
        case TriangleType::TriangleStrip: {
            glTriangleType = GL_TRIANGLE_STRIP; break;
        }
    }

    glDrawElements(glTriangleType, indicesCount, GL_UNSIGNED_INT, nullptr);
}

void GraphicsEngine::setFaceCulling(const CullType &type) {
    auto cullType = GL_BACK;

    switch (type) {
        case CullType::FrontFace: { cullType = GL_FRONT; break; }
        case CullType::BackFace: { cullType = GL_BACK; break; }
        case CullType::Both: { cullType = GL_FRONT_AND_BACK; break; }
    }

    glEnable(GL_CULL_FACE);
    glCullFace(cullType);
}

void GraphicsEngine::setWindingOrder(const WindingOrder &order) {
    auto orderType = GL_CW;

    switch (order) {
        case WindingOrder::Clockwise: { orderType = GL_CW; break; }
        case WindingOrder::CounterClockwise: { orderType = GL_CCW; break; }
    }

    glFrontFace(orderType);
}

int GraphicsEngine::countFPS() {
    auto currentTime = clock::now();
    double deltaTime = std::chrono::duration<double>(currentTime - lastTime).count();
    lastTime = currentTime;

    frameCount++;
    elapsedTime += deltaTime;

    if (elapsedTime >= 1.0) {
        fps = frameCount / elapsedTime;
        frameCount = 0;
        elapsedTime = 0.0;
    }
    return fps;
}