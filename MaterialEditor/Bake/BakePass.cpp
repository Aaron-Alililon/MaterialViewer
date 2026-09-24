#include "Bake/BakePass.h"

BakePass::BakePass(std::weak_ptr<rcore::Window> const& window) :
  m_window{ window },
  m_depthStencilState{ rcore::Preset3D::makeDisabledDepthStencilDescription() },
  m_rasterizerState{ rcore::Preset3D::makeNoCullingRasterDescription() }
{
  prepareCache();
}

void BakePass::pruneCache() {
  std::error_code ec;
  for (auto const& file : std::filesystem::directory_iterator(s_cacheDir, ec)) {
    if (file.path().extension() != ".dds") continue;
    if (!s_usedCacheFiles.contains(file.path().filename())) {
      std::filesystem::remove(file.path(), ec);
    }
  }
}

void BakePass::bake() {
  if (m_baked) {
    RCORE_LOG(rcore::WARN, "Tried baking one bakepass multiple times - Use rebake() if this is intentional");
    return;
  }

  m_cacheKey = makeCacheKey();
  m_cacheEntries = getCacheEntries();

  for (auto const& entry : m_cacheEntries) s_usedCacheFiles.insert(entry.first);

  bool cacheLoaded = false;
  if (checkCache()) {
    cacheLoaded = loadCachedData();
  }
  
  if (!cacheLoaded) {
    rebake();
  }

  m_baked = true;
}

void BakePass::rebake() {
  bindBakeState();

  if (
    !setupRenderTargets() ||
    !bindSourceData() ||
    !draw()
  ) {
    RCORE_LOG(rcore::ERR, "An error occured during baking");
  } else {
    storeCachedData();
  }

  restoreContext();
}

std::string BakePass::getCacheKey() const {
  return m_cacheKey;
}

uint64_t BakePass::hashData(void const* data, size_t size, uint64_t h) const {
  auto const* p = static_cast<unsigned char const*>(data);
  for (size_t i = 0; i < size; i++) { h ^= p[i]; h *= 1099511628211ull; }
  return h;
}

void BakePass::prepareCache() const {
  std::filesystem::create_directories(s_cacheDir);
}

bool BakePass::checkCache() const {
  bool filesExist = true;

  for (auto const& entry : m_cacheEntries) {
    filesExist &= std::filesystem::exists(s_cacheDir / entry.first);
  }

  return filesExist;
}

bool BakePass::loadCachedData() const {
  bool filesValid = true;

  for (auto const& entry : m_cacheEntries) {
    *entry.second = std::make_shared<rcore::RenderTarget>(s_cacheDir / entry.first);
    filesValid &= (*entry.second)->isValid();
  }

  return filesValid;
}

bool BakePass::storeCachedData() const {
  bool filesValid = true;

  for (auto const& entry : m_cacheEntries) {
    filesValid &= (*entry.second)->storeAsDDS(s_cacheDir / entry.first);
  }

  return filesValid;
}

void BakePass::bindBakeState() {
  m_depthStencilState.bind();
  m_rasterizerState.bind();
}

void BakePass::restoreContext() {
  auto lockedWindow = m_window.lock();
  if (!lockedWindow) {
    RCORE_LOG(rcore::ERR, "Tried accessing invalid window pointer while baking");
    return;
  }

  lockedWindow->bindContextStates();
  lockedWindow->activateContext();
}