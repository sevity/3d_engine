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
# import pudb; pudb.set_trace()
# 원근 투영 (Perspective Projection)
projected_points = []
for point in cube_points:
    x, y, z = point
    # Z=1 평면에 투영
    projected_points.append([x / z, y / z])

# numpy 배열로 변환
projected_points = np.array(projected_points)

# 2D 투영 점 플롯
plt.scatter(projected_points[:, 0], projected_points[:, 1])

# 파일로 저장
plt.show()
