import OffScreenFFR


def testOffScreenFFR():
    winSizeX = 2000
    winSizeY = 2000
    elevationAngle=0
    azimuthAngle=0
    ratio=0.5
    useHorizontalViewAngle = False

    fileName = "/home/liuxiaobo/OffScreenFFR/Source/Testing/Data/specific_cut-0.14.vtp"
    pngFile = "/home/liuxiaobo/OffScreenFFR/Distribute/Build/Wrapping/Python/screenshot.png"
    OffScreenFFR.VtkUtils_OffScreenRender(fileName, winSizeX, winSizeY, elevationAngle, azimuthAngle, ratio, useHorizontalViewAngle, pngFile)


def testFFRPicker():
    inputFileName = "/home/liuxiaobo/OffScreenFFR/Source/Testing/Data/specific_cut-0.14.vtp"
    sizeX = 2000
    sizeY = 2000
    angleIndex = 0

    picker = OffScreenFFR.FFRPicker()
    picker.SetInputFile(inputFileName)
    picker.SetRenderParas(sizeX, sizeY, angleIndex)

    result = picker.Render()

    if result:
        ffr = picker.PickFFR(760*2, 392*2)
        print("ffr:", ffr)
        ffr = picker.PickFFR(585*2, 493*2)
        print("ffr:", ffr)
        ffr = picker.PickFFR(447*2, 600*2)
        print("ffr:", ffr)
        ffr = picker.PickFFR(506*2, 450*2)
        print("ffr:", ffr)

    if result:
        ffr = picker.PickFFR(760*2, 392*2)
        print("ffr:", ffr)
        posVec = picker.GetPointPosVec()
        for i in range(len(posVec)):
            pos = posVec[i]
            print("angle index:", i)
            print("pos :", pos[0], ",", pos[1],  ",", pos[2])
