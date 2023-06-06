import numpy as np
import pygame
from pygame.locals import *
from OpenGL.GL import *
from OpenGL.GLU import *

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

def cube():
    glBegin(GL_POINTS)
    for point in rotated_points:
        glVertex3fv(point)
    glEnd()

def main():
    pygame.init()
    display = (800, 600)
    pygame.display.set_mode(display, DOUBLEBUF|OPENGL)
    gluPerspective(45, (display[0]/display[1]), 0.1, 50.0)
    glTranslatef(0.0, 0.0, -5)

    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                quit()

        glRotatef(1, 3, 1, 1)
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT)
        cube()
        pygame.display.flip()
        pygame.time.wait(10)


if __name__ == "__main__":
    main()
