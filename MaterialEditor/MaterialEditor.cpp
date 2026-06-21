#include "Rcore.h"
#include "EditorLayer.h"
#include "Descriptors.inl"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
  rcore::DeviceDesc devDesc;
  rcore::initDevice(devDesc);

  rcore::WindowDesc windDesc = getWindowDesc();
  auto mainWindow = rcore::makeWindow(windDesc);
  // auto subWindow = rcore::makeWindow(windDesc);

  rcore::D3DContextDesc ctxDesc = getContextDesc(windDesc.width(), windDesc.height());
  rcore::makeD3D11Context(mainWindow, ctxDesc);

  mainWindow.lock()->addLayer<EditorLayer>();

  rcore::init();
}