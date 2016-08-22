#pragma once

#include "drape/color.hpp"
#include "drape/glsl_types.hpp"
#include "drape/glstate.hpp"
#include "drape/overlay_handle.hpp"
#include "drape/render_bucket.hpp"
#include "drape/texture_manager.hpp"

#include "geometry/polyline2d.hpp"

#include "std/map.hpp"
#include "std/set.hpp"
#include "std/string.hpp"
#include "std/vector.hpp"
#include "std/unordered_map.hpp"

namespace df
{

enum TrafficSpeedBucket
{
  Normal = 3,
  Slow = 2,
  VerySlow = 1
};

struct TrafficSegmentData
{
  string m_id;
  TrafficSpeedBucket m_speedBucket;

  TrafficSegmentData() = default;
  TrafficSegmentData(string const & id, TrafficSpeedBucket const & speedBucket)
    : m_id(id)
    , m_speedBucket(speedBucket)
  {}
};

struct TrafficRenderData
{
  dp::GLState m_state;
  drape_ptr<dp::RenderBucket> m_bucket;
  m2::RectF m_texRect;
  TrafficRenderData(dp::GLState const & state) : m_state(state) {}
};

struct TrafficStaticVertex
{
  using TPosition = glsl::vec3;
  using TNormal = glsl::vec4;

  TrafficStaticVertex() = default;
  TrafficStaticVertex(TPosition const & position, TNormal const & normal)
    : m_position(position)
    , m_normal(normal)
  {}

  TPosition m_position;
  TNormal m_normal;
};

struct TrafficDynamicVertex
{
  using TTexCoord = glsl::vec2;

  TrafficDynamicVertex() = default;
  TrafficDynamicVertex(TTexCoord const & color)
    : m_colorTexCoord(color)
  {}

  TTexCoord m_colorTexCoord;
};

class TrafficHandle : public dp::OverlayHandle
{
  using TBase = dp::OverlayHandle;

public:
  TrafficHandle(string const & segmentId, m2::RectD const & bbox,
                glsl::vec2 const & texCoord, size_t verticesCount);

  void GetAttributeMutation(ref_ptr<dp::AttributeBufferMutator> mutator) const override;
  bool Update(ScreenBase const & screen) override;
  bool IndexesRequired() const override;
  m2::RectD GetPixelRect(ScreenBase const & screen, bool perspective) const override;
  void GetPixelShape(ScreenBase const & screen, bool perspective, Rects & rects) const override;

  void SetTexCoord(glsl::vec2 const & texCoord);
  string const & GetSegmentId() const;
  m2::RectD const & GetBoundingBox() const;

private:
  string m_segmentId;
  m2::RectD m_bbox;
  vector<glsl::vec2> m_buffer;
  mutable bool m_needUpdate;
};

class TrafficGenerator final
{
public:
  TrafficGenerator() = default;

  void AddSegment(string const & segmentId, m2::PolylineD const & polyline);

  vector<TrafficSegmentData> GetSegmentsToUpdate(vector<TrafficSegmentData> const & trafficData) const;

  void GetTrafficGeom(ref_ptr<dp::TextureManager> textures,
                      vector<TrafficSegmentData> const & trafficData,
                      vector<TrafficRenderData> & data);

  void ClearCache();

  bool IsColorsCacheRefreshed() const { return m_colorsCacheRefreshed; }
  unordered_map<int, glsl::vec2> ProcessCacheRefreshing();

private:
  using TSegmentCollection = map<string, m2::PolylineD>;

  void GenerateSegment(dp::TextureManager::ColorRegion const & colorRegion,
                       m2::PolylineD const & polyline,
                       vector<TrafficStaticVertex> & staticGeometry,
                       vector<TrafficDynamicVertex> & dynamicGeometry);
  void FillColorsCache(ref_ptr<dp::TextureManager> textures);

  TSegmentCollection m_segments;

  set<string> m_segmentsCache;
  unordered_map<int, dp::TextureManager::ColorRegion> m_colorsCache;
  bool m_colorsCacheRefreshed = false;
};

} // namespace df
