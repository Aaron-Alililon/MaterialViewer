#ifndef BAKE_PASS_H
#define BAKE_PASS_H

#include <set>

#include "Core/Preset3D.h"
#include "Window/Window.h"
#include "Resource/RenderTarget.h"

class BakePass {
public:
  using CacheEntry = std::pair<std::filesystem::path, std::shared_ptr<rcore::RenderTarget>*>;

public:
  BakePass(std::weak_ptr<rcore::Window> const& window);
  virtual ~BakePass() = default;

public:
  static void pruneCache();

  void bake();
  void rebake();

  std::string getCacheKey() const;

protected:
  virtual std::string makeCacheKey() const = 0;
  virtual std::vector<CacheEntry> getCacheEntries() = 0;
  virtual bool setupRenderTargets() = 0;
  virtual bool bindSourceData() = 0;
  virtual bool draw() = 0;

  uint64_t hashData(void const* data, size_t size, uint64_t h = 14695981039346656037ull) const;

private:
  void prepareCache() const;
  bool checkCache() const;
  bool loadCachedData() const;
  bool storeCachedData() const;
  void bindBakeState();
  void restoreContext();

protected:
  inline static std::set<std::filesystem::path> s_usedCacheFiles;
  inline static const std::filesystem::path s_cacheDir = "cache";

  std::string m_cacheKey;

private:
  bool m_baked = false;
  std::vector<CacheEntry> m_cacheEntries;
  std::weak_ptr<rcore::Window> m_window;
  rcore::DepthStencilState m_depthStencilState;
  rcore::RasterizerState m_rasterizerState;
};

#endif