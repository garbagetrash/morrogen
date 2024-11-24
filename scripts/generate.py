#!/usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt
import opensimplex as osx


class Fbm:
    def __init__(self, base_size=2.0, noctaves=4, seed=1337):
        self.base_size = base_size
        self.noctaves = noctaves
        self.seed = seed

    def generate(self, x, y, offsetx=0, offsety=0):
        osx.seed(self.seed)
        xx = np.arange(x) + offsetx
        yy = np.arange(y) + offsety
        z = np.zeros((x, y), dtype=float)
        for i in range(self.noctaves):
            amplitude = 2**i
            scale = (2**i) * self.base_size
            z += amplitude * osx.noise2array(xx / scale, yy / scale)
        return z / (2**self.noctaves)


class Cell:
    def __init__(self):
        self.grid = np.zeros((65, 65), dtype=np.int8)


class Map:
    def __init__(self):
        self.cells = {}

    def get_height_at(self, x, y):
        cx = x // 65
        cy = y // 65
        xx = x % 65
        yy = y % 65
        return self.cells[(cx, cy)].grid[xx, yy]

    def plot_heightmap(self):
        z = generate_full_map(self)
        for k, v in self.cells.items():
            z[65*k[0]:65*(k[0]+1), 65*k[1]:65*(k[1]+1)] = v.grid

        plt.figure()
        plt.imshow(z)
        plt.grid(True)


def test_create_map(seed=1337, size=48.0):
    osx.seed(seed)
    m = Map()
    cx = 0
    cy = 0

    for cx in range(5):
        for cy in range(5):
            m.cells[(cy, cx)] = Cell()
            z = Fbm().generate(65, 65, 65*cx, 65*cy)
            m.cells[(cy, cx)].grid = np.array(127.0 * z, dtype=np.int8)

    m.plot_heightmap()

    return m


def generate_full_map(m):
    xs = []
    ys = []
    for key in m.cells:
        xs.append(key[0])
        ys.append(key[1])
    xs.sort()
    ys.sort()

    return np.zeros((65*(xs[-1] - xs[0] + 1), 65*(ys[-1] - ys[0] + 1)), dtype=np.int8)


def generate_settlement_points(m, n_settlements):
    # Higher values near water, away from other settlements
    values = generate_full_map(m)
    for x in range(values.shape[1]):
        for y in range(values.shape[0]):
            z = m.get_height_at(x, y)
            values[x, y] = -np.abs(z) #+ dist_to_settlement(s)

    plt.figure()
    plt.imshow(values)
    plt.title("Settlement Values")
    plt.grid(True)



if __name__ == "__main__":
    m = test_create_map()
    generate_settlement_points(m, 1)
    plt.show()
