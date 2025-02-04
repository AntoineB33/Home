class Settings:
    def __init__(self):
        self.sound_enabled = True
        self.volume = 1.0  # Default volume (1.0 = 100%)

    def toggle_sound(self):
        self.sound_enabled = not self.sound_enabled

    def adjust_volume(self, level):
        self.volume = level
