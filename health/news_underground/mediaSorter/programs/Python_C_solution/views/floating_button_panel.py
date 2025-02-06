# views/floating_button_panel.py

from PyQt5.QtWidgets import QWidget, QPushButton, QHBoxLayout
from PyQt5.QtCore import Qt, QPoint

class FloatingButtonPanel(QWidget):
    """
    A small floating panel with buttons. It can be moved around within its parent
    (which should be a QTableView) but will not be allowed to overlap the scroll bars.
    """
    def __init__(self, parent, *args, **kwargs):
        # Using the parent QTableView as parent means our coordinates are relative to it.
        super().__init__(parent, *args, **kwargs)
        # Ensure that the style sheet background is actually drawn.
        self.setAttribute(Qt.WA_StyledBackground, True)
        # Set window flags so that it floats over the parent widget.
        self.setWindowFlags(Qt.SubWindow)
        # Set a light gray background and a border.
        self.setStyleSheet("background-color: lightgray; border: 1px solid black;")
        self._drag_start_position = None
        self._init_ui()

    def _init_ui(self):
        # Create some example buttons.
        self.button1 = QPushButton("Button 1", self)
        self.button2 = QPushButton("Button 2", self)
        # Create a horizontal layout with padding.
        layout = QHBoxLayout(self)
        padding = 40
        layout.setContentsMargins(padding, padding, padding, padding)  # Padding around the buttons.
        layout.setSpacing(padding)
        layout.addWidget(self.button1)
        layout.addWidget(self.button2)
        self.setLayout(layout)
        # Resize the widget to fit the layout and its contents.
        self.adjustSize()

    def mousePressEvent(self, event):
        if event.button() == Qt.LeftButton:
            self._drag_start_position = event.pos()
        super().mousePressEvent(event)

    def mouseMoveEvent(self, event):
        if event.buttons() & Qt.LeftButton and self._drag_start_position:
            # Calculate the new position relative to the parent's coordinate space.
            new_pos = self.pos() + (event.pos() - self._drag_start_position)
            # Enforce boundaries so the floating panel does not cover the scroll bars.
            parent = self.parent()
            if parent:
                parent_width = parent.width()
                parent_height = parent.height()
                # Get scroll bar widgets (they are children of QTableView)
                vertical_sb = parent.verticalScrollBar()
                horizontal_sb = parent.horizontalScrollBar()
                # Determine scroll bar dimensions if visible.
                vertical_width = vertical_sb.width() if vertical_sb and vertical_sb.isVisible() else 0
                horizontal_height = horizontal_sb.height() if horizontal_sb and horizontal_sb.isVisible() else 0

                # Define allowed boundaries.
                max_x = parent_width - vertical_width - self.width()
                max_y = parent_height - horizontal_height - self.height()

                # Clamp the new position within allowed boundaries.
                new_x = max(0, min(new_pos.x(), max_x))
                new_y = max(0, min(new_pos.y(), max_y))
                new_pos = QPoint(new_x, new_y)
            self.move(new_pos)
        super().mouseMoveEvent(event)

    def mouseReleaseEvent(self, event):
        self._drag_start_position = None
        super().mouseReleaseEvent(event)
