#include "MarchingCubes.hpp"

#include <functional>

namespace MarchingCubes
{
    /**
     * @brief Gets the singleton instance for this class
     * @return Single instance for this class
     */
    MarchingCubes& MarchingCubes::GetInstance()
    {
        static MarchingCubes instance;
        return instance;
    }

    /**
     * @brief Constructor
     */
    MarchingCubes::MarchingCubes()
    {
    }

    /**
     * @brief Gets the resulting mesh upon performing marching cubes
     * @param[in] signedDistanceFunc Signed distance function
     * @param[in] bounds Shape bounds
     * @param[in] cellSize Cell size
     * @param[out] outputTriangles Vector where the triangles will be placed
     */
    void MarchingCubes::GetMesh(std::function<float(float, float, float)> signedDistanceFunc, const AABB& bounds, float cellSize, std::vector<Triangle>& outputTriangles)
    {
        for (float x = bounds.min[0]; x < bounds.max[0]; x += cellSize)
        {
            for (float y = bounds.min[1]; y < bounds.max[1]; y += cellSize)
            {
                for (float z = bounds.min[2]; z > bounds.max[2]; z -= cellSize) // Right-handed system
                {
                    GetCellTriangles(signedDistanceFunc, x, y, z, cellSize, outputTriangles);
                }
            }
        }
    }

    /**
     * @brief Gets the cell triangles based on the resulting cell configuration calculated from the provided function
     * @param[in] signedDistanceFunc Signed distance function
     * @param[in] cellX X-position of the cell
     * @param[in] cellY Y-position of the cell
     * @param[in] cellZ Z-position of the cell
     * @param[in] cellSize Cell size
     * @param[out] outputTriangles Vector where the triangles will be placed 
     */
    void MarchingCubes::GetCellTriangles(std::function<float(float, float, float)> signedDistanceFunc, float cellX, float cellY, float cellZ, float cellSize, std::vector<Triangle>& outputTriangles)
    {
        float values[8];
        for (int i = 0; i < 8; ++i)
        {
            values[i] = signedDistanceFunc(cellX + vertexPositionOffsets[i].x * cellSize,
                                           cellY + vertexPositionOffsets[i].y * cellSize,
                                           cellZ + vertexPositionOffsets[i].z * cellSize);
        }

        int caseIndex = 0;
        for (int i = 0; i < 8; ++i)
        {
            if (values[i] > 0.0f)
            {
                caseIndex |= 1 << i;
            }
        }

        unsigned char caseClass = regularCellClass[caseIndex];

        RegularCellData cellData = regularCellData[caseClass];
        if (cellData.GetVertexCount() == 0)
        {
            return;
        }

        Triangle triangle;
        for (int i = 0; i < cellData.GetTriangleCount(); ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                int index = cellData.vertexIndex[i * 3 + j];

                int ev0 = (regularVertexData[caseIndex][index] & 0xF0) >> 4;
                int ev1 = regularVertexData[caseIndex][index] & 0x0F;

                glm::vec3 vPos = ((vertexPositionOffsets[ev0] + vertexPositionOffsets[ev1]) / 2.0f) * cellSize;
                vPos.x += cellX;
                vPos.y += cellY;
                vPos.z += cellZ;
                triangle.vertices[j] = vPos;
            }
            outputTriangles.push_back(triangle);
        }
    }
}
