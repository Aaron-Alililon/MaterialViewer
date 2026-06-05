#include "Rcore.h"
#include "EditorLayer.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
  rcore::DeviceDesc devDesc;
  rcore::initDevice(devDesc);

  rcore::WindowDesc windDesc;
  windDesc.name(L"Material Editor");
  windDesc.width(800);
  windDesc.height(800);
  rcore::WindowView mainWindow = rcore::makeWindow(windDesc);

  rcore::D3DContextDesc ctxDesc;
  ctxDesc.targetFps(144);
  rcore::makeD3D11Context(mainWindow, ctxDesc);

  mainWindow.addLayer<EditorLayer>();

  rcore::init();
}