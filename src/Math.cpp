#include"Math.h"
#include<cstring>
#include<cassert>
void bezierCurve(const std::vector<glm::vec3>& input,
                 std::vector<glm::vec3>& output,
                 const float step) {

    assert(step > 0.0f);
    if(input.size() == 1) return;
    if(input.size() == 2) {
        output.push_back(input[0]), output.push_back(input[1]);
        return;
    }

    output.clear();
    glm::vec3* tmp = new glm::vec3[input.size()];

    for(float t = 0.0f; t < 1.0f; t += step) {
        memcpy(tmp, input.data(), input.size()* sizeof(glm::vec3));
        int i = input.size() - 1;
        while (i > 0) {
            for (int k = 0; k < i; k++)
                tmp[k] = tmp[k] + t * ( tmp[k + 1] - tmp[k] );
            i--;
        }
        output.push_back(tmp[0]);
    }
    delete[] tmp;
}















