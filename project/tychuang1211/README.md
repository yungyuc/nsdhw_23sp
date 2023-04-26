# Improve modmesh camera control 

## Basic Information

Implement a new QT camera controller class to improve current camera control of modmesh.

GitHub repository: https://github.com/tychuang1211/modmesh/tree/70-improve-camera-control

## Problem to Solve

Make camera control more straight forward to use.
Add functionality:
1. Resetting.
2. WSAD.
3. Change of origin.
4. ...

## Prospective Users

modmesh users

## System Architecture

Derive a class from `Qt3DExtras::QAbstractCameraController` and implement the virtual function `moveCamera` to suits our needs.

## API Description

> TODO

## Engineering Infrastructure

`Git` for version control
`Github Action` for running tests
`Pytest` for testing

## Schedule

Week 1 (04/10): Research QT camera controller source code

Week 2 (04/17): Implement a new camera controller class, integrate the feature of QOrbitCameraControl and QFirstPersonCameraControl

Week 3 (04/24): Support more straight forward controls (e.g. supports WASDQE instead of Arrow/PageUpDown)

Week 4 (05/01): Testing & refactoring

Week 5 (05/08): Add functionality (e.g. resetting, change of origin)

Week 6 (05/15): Add functionality (e.g. resetting, change of origin)

Week 7 (05/22): Testing & refactoring

Week 8 (05/29): Prepare for presentation

## References

* [Improve camera control #70](https://github.com/solvcon/modmesh/issues/70) 
* [feat: Use FPSCamera to improve control #112](https://github.com/solvcon/modmesh/pull/112)
* [qt/qt3d](https://github.com/qt/qt3d)
* [Qt 3D Documentation](https://doc.qt.io/qt-6/qt3d-index.html)
