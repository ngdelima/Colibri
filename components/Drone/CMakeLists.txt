idf_component_register(SRCS "Drone.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES Commands
                    REQUIRES Communication
                    REQUIRES Control)

