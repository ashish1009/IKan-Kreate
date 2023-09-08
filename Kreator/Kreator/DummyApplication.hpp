//
//  DummyApplication.hpp
//  Kreator
//
//  Created by Ashish . on 08/09/23.
//

#include <glad/glad.h>

namespace Kreator
{
  unsigned int planeVAO, planeVBO;
  unsigned int cubeVAO, cubeVBO;
  class DummyApp : public Application
  {
  public:
    DummyApp(const Application::Specification& appSpec, const std::string& clientResourcePath, const std::string& startupProject)
    : Application(appSpec)
    {
      shader = Shader::Create("/Users/ashish./iKan_storage/Github/Product/IKan-Kreate/IKan/Assets/Shaders/PBR_StaticShader.glsl");
      stencil = Shader::Create("/Users/ashish./iKan_storage/Github/Product/IKan-Kreate/IKan/Assets/Shaders/MeshStencilShader.glsl");
    }
    
    ~DummyApp()
    {
    }
    
    void OnInit() override
    {
      camera.SetViewportSize(1600, 900);
      // set up vertex data (and buffer(s)) and configure vertex attributes
      // ------------------------------------------------------------------
      float cubeVertices[] = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
      };
      float planeVertices[] = {
        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
        5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
        
        5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
        5.0f, -0.5f, -5.0f,  2.0f, 2.0f
      };
      // cube VAO
      glGenVertexArrays(1, &cubeVAO);
      glGenBuffers(1, &cubeVBO);
      glBindVertexArray(cubeVAO);
      glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
      glBindVertexArray(0);
      // plane VAO
      glGenVertexArrays(1, &planeVAO);
      glGenBuffers(1, &planeVBO);
      glBindVertexArray(planeVAO);
      glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
      glBindVertexArray(0);
    }
    
    void OnShutdown() override
    {
      
    }
    
    void OnUpdate(TimeStep ts) override
    {
      camera.OnUpdate(ts);
      camera.SetActive(true);

      // render
      // ------
      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // don't forget to clear the stencil buffer!
      
      // set uniforms
      glm::mat4 model = glm::mat4(1.0f);
      
      
      // draw floor as normal, but don't write the floor to the stencil buffer, we only care about the containers.
      // We set its mask to 0x00 to not write to the stencil buffer.
      glStencilMask(0x00);
      
      // floor
      shader->Bind();
      shader->SetUniformMat4("u_ViewProjection", camera.GetUnReversedViewProjection());
      shader->SetUniformMat4("u_Transform", model);

      glBindVertexArray(planeVAO);
      glDrawArrays(GL_TRIANGLES, 0, 6);
      glBindVertexArray(0);
      
      // 1st. render pass, draw objects as normal, writing to the stencil buffer
      // --------------------------------------------------------------------
      glStencilFunc(GL_ALWAYS, 1, 0xFF);
      glStencilMask(0xFF);
      
      // cubes
      model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
      shader->Bind();
      shader->SetUniformMat4("u_ViewProjection", camera.GetUnReversedViewProjection());
      shader->SetUniformMat4("u_Transform", model);
      glBindVertexArray(cubeVAO);
      glDrawArrays(GL_TRIANGLES, 0, 36);
      glBindVertexArray(0);

      // 2nd. render pass: now draw slightly scaled versions of the objects, this time disabling stencil writing.
      // Because the stencil buffer is now filled with several 1s. The parts of the buffer that are 1 are not drawn, thus only drawing
      // the objects' size differences, making it look like borders.
      // -----------------------------------------------------------------------------------------------------------------------------
      glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
      glStencilMask(0x00);
      glDisable(GL_DEPTH_TEST);

      float scale = 1.2f;
      model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
      model = glm::scale(model, glm::vec3(scale, scale, scale));

      stencil->Bind();
      stencil->SetUniformMat4("u_ViewProjection", camera.GetUnReversedViewProjection());
      stencil->SetUniformMat4("u_Transform", model);

      // cubes
      glBindVertexArray(cubeVAO);
      glDrawArrays(GL_TRIANGLES, 0, 36);
      glBindVertexArray(0);

      glStencilMask(0xFF);
      glStencilFunc(GL_ALWAYS, 0, 0xFF);
      glEnable(GL_DEPTH_TEST);
    }
    
    void OnEvent(Event& e) override
    {
      camera.OnEvent(e);
    }

  private:
    Ref<Shader> shader, stencil;
    EditorCamera camera = EditorCamera(45.0f, 1280.0f, 720.0f, 0.1f, 10000.0f);
  };
} // namespace Kreator
