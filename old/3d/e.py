import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

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

# 정육면체 꼭지점 연결 순서 정의
edges = [
    (0, 1), (1, 2), (2, 3), (3, 0),  # 아래면
    (4, 5), (5, 6), (6, 7), (7, 4),  # 위면
    (0, 4), (1, 5), (2, 6), (3, 7)   # 사이드면
]

# 무게 중심 계산
center_of_mass = np.mean(cube_points, axis=0)

# 무게 중심을 원점으로 이동
cube_points = cube_points - center_of_mass

# 그림 설정
fig, ax = plt.subplots()

# 애니메이션 업데이트 함수
def update(num):
    ax.clear()

    # 회전 각도
    theta = np.radians(num)

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
        projected_points.append([x / (z+5), y / (z+5)])

    # numpy 배열로 변환
    projected_points = np.array(projected_points)

    # 2D 투영 점 플롯 및 연결
    for edge in edges:
        points = projected_points[list(edge)]
        ax.plot(points[:, 0], points[:, 1])

    # 축 설정
    ax.set_xlim([-1, 1])
    ax.set_ylim([-1, 1])

# 애니메이션 생성
ani = animation.FuncAnimation(fig, update, frames=360, interval=1)

# 애니메이션 표시
plt.show()

