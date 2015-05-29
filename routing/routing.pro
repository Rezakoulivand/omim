# Base functions project.
TARGET = routing
TEMPLATE = lib
CONFIG += staticlib warn_on c++11

ROOT_DIR = ..

include($$ROOT_DIR/common.pri)

DEFINES *= BOOST_ERROR_CODE_HEADER_ONLY
INCLUDEPATH += $$ROOT_DIR/3party/jansson/src \
               $$ROOT_DIR/3party/osrm/osrm-backend/include \
               $$ROOT_DIR/3party/osrm/osrm-backend/third_party

SOURCES += \
    astar_router.cpp \
    async_router.cpp \
    cross_mwm_road_graph.cpp \
    cross_mwm_router.cpp \
    cross_routing_context.cpp \
    features_road_graph.cpp \
    nearest_edge_finder.cpp \
    online_cross_fetcher.cpp \
    osrm2feature_map.cpp \
    osrm_engine.cpp \
    osrm_online_router.cpp \
    osrm_router.cpp \
    road_graph.cpp \
    road_graph_router.cpp \
    route.cpp \
    routing_mapping.cpp \
    turns.cpp \
    turns_generator.cpp \
    vehicle_model.cpp \

HEADERS += \
    astar_router.hpp \
    async_router.hpp \
    base/astar_algorithm.hpp \
    base/graph.hpp \
    cross_mwm_road_graph.hpp \
    cross_mwm_router.hpp \
    cross_routing_context.hpp \
    features_road_graph.hpp \
    nearest_edge_finder.hpp \
    online_cross_fetcher.hpp \
    osrm2feature_map.hpp \
    osrm_data_facade.hpp \
    osrm_engine.hpp \
    osrm_online_router.hpp \
    osrm_router.hpp \
    road_graph.hpp \
    road_graph_router.hpp \
    route.hpp \
    router.hpp \
    routing_mapping.h \
    turns.hpp \
    turns_generator.hpp \
    vehicle_model.hpp \
