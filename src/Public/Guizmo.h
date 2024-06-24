#pragma once
#include <memory>
#include "Model.h"
namespace HC {

    class Guizmo {
    public:
        inline static Guizmo* GetInstance() {
            static Guizmo instance;
            return &instance;
        }

        void DrawLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color) {
            vec.push_back({from, color});
            vec.push_back({to, color});
            my_indices.push_back(my_indices.size());
            my_indices.push_back(my_indices.size());
        }

        void DrawCube(const glm::vec3& base_position, const glm::vec3& last_position,const glm::vec3& color) {
            DrawLine(base_position, {last_position.x, base_position.y, base_position.z}, color);
            DrawLine(base_position, {base_position.x, last_position.y, base_position.z}, color);
            DrawLine(base_position, {base_position.x, base_position.y, last_position.z}, color);

            DrawLine({base_position.x, last_position.y, last_position.z}, {base_position.x, base_position.y, last_position.z}, color);
            DrawLine({last_position.x, base_position.y, last_position.z}, {base_position.x, base_position.y, last_position.z}, color);
            DrawLine({last_position.x, base_position.y, last_position.z}, {last_position.x, base_position.y, base_position.z}, color);

            DrawLine({last_position.x, last_position.y, base_position.z}, {last_position.x, base_position.y, base_position.z}, color);
            DrawLine({last_position.x, last_position.y, base_position.z}, {base_position.x, last_position.y, base_position.z}, color);
            DrawLine({last_position.x, last_position.y, base_position.z}, {last_position.x, last_position.y, last_position.z}, color);

            DrawLine({last_position.x, last_position.y, last_position.z}, {base_position.x, last_position.y, last_position.z}, color);
            DrawLine({last_position.x, last_position.y, last_position.z}, {last_position.x, base_position.y, last_position.z}, color);
            DrawLine({last_position.x, last_position.y, last_position.z}, {last_position.x, last_position.y, base_position.z}, color);

            DrawLine({base_position.x, last_position.y, base_position.z}, {base_position.x, last_position.y, last_position.z}, color);


        }

        void ProcessDraw() {
            if(vec.empty()) {
                return;
            }
            model = std::make_unique<Model>(vec, my_indices);
            glLineWidth(2.0f);
            model->SetMode(GL_LINES);
            model->Draw();
            vec.clear();
            my_indices.clear();
        }

        std::unique_ptr<Model> model;
        std::vector<Vertex> vec;
        std::vector<GLuint> my_indices;
    };


}


