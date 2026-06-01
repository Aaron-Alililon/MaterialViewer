#include "Rcore.h"
#include "EditorLayer.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
  rcore::WindowDesc windDesc;
  windDesc.name(L"Material Editor");
  windDesc.width(800);
  windDesc.height(800);

  rcore::WindowView mainWindow = rcore::makeWindow(windDesc);

  mainWindow.addLayer<EditorLayer>();

  rcore::DeviceDesc devDesc;

  rcore::init(devDesc);
}