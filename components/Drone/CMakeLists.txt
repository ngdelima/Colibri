idf_component_register(SRCS "Drone.cpp"
					PRIV_REQUIRES driver
                    INCLUDE_DIRS "include"
                    REQUIRES Commands
                    REQUIRES Communication
                    REQUIRES Control)

