from conans import ConanFile, CMake

class AutoDriveConan(ConanFile):
    name = "imaging"
    version = "1.0"
    settings = None
    exports = "*"
    requires = "lms_camera_importer/1.0@lms/stable"

    def imports(self):
        self.copy("*.so",dst="bin")
