import numpy as np
import matplotlib.pyplot as plt

# 3D 정육면체 꼭지점 정의
cube_points = np.array([
    [-1, -1, 2],
    [1, -1, 2],
    [1, 1, 2],
    [-1, 1, 2],
    [-1, -1, 3],
    [1, -1, 3],
    [1, 1, 3],
    [-1, 1, 3],
])

# 무게 중심 계산
center_of_mass = np.mean(cube_points, axis=0)

# 무게 중심을 원점으로 이동
cube_points = cube_points - center_of_mass

# 회전 각도 (예: 45도)
theta = np.radians(45)

# y축을 중심으로 하는 회전 행렬
rotation_matrix = np.array([
    [np.cos(theta), 0, np.sin(theta)],
    [0, 1, 0],
    [-np.sin(theta), 0, np.cos(theta)]
])

# 회전 적용
rotated_points = np.dot(cube_points, rotation_matrix)

# 원래의 위치로 이동
rotated_points = rotated_points + center_of_mass

# 원근 투영 (Perspective Projection)
projected_points = []
for point in rotated_points:
    x, y, z = point
    # Z=1 평면에 투영
    projected_points.append([x / z, y / z])

# numpy 배열로 변환
projected_points = np.array(projected_points)

# 2D 투영 점 플롯
plt.scatter(projected_points[:, 0], projected_points[:, 1])

# 파일로 저장
plt.show()
