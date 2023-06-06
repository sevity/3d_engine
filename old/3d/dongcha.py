import numpy as np
import matplotlib.pyplot as plt

# 3D 정육면체 꼭지점 정의 (동차 좌표계로 확장)
cube_points = np.array([
    [-1, -1, 2, 1],
    [1, -1, 2, 1],
    [1, 1, 2, 1],
    [-1, 1, 2, 1],
    [-1, -1, 4, 1],
    [1, -1, 4, 1],
    [1, 1, 4, 1],
    [-1, 1, 4, 1],
])

# 무게 중심 계산 (동차 좌표계 요소는 포함하지 않음)
center_of_mass = np.mean(cube_points[:, :3], axis=0)

# 회전 각도 (예: 45도)
theta = np.radians(45)

# 변환 행렬 생성 (동차 좌표계)
transformation_matrix = np.array([
    [np.cos(theta), 0, np.sin(theta), -center_of_mass[0]],
    [0, 1, 0, -center_of_mass[1]],
    [-np.sin(theta), 0, np.cos(theta), -center_of_mass[2]],
    [0, 0, 0, 1]
])

# 변환 적용
transformed_points = np.dot(cube_points, transformation_matrix)

# 원근 투영 (Perspective Projection)
projected_points = []
for point in transformed_points:
    x, y, z, w = point
    # Z=1 평면에 투영
    projected_points.append([x / z, y / z])

# numpy 배열로 변환
projected_points = np.array(projected_points)

# 2D 투영 점 플롯
plt.scatter(projected_points[:, 0], projected_points[:, 1])

# 파일로 저장
plt.show()

