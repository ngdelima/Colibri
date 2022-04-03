idf_component_register(SRCS "main.cpp"
					PRIV_REQUIRES driver
                    INCLUDE_DIRS "."
                    REQUIRES Communication
                    REQUIRES Drone)
