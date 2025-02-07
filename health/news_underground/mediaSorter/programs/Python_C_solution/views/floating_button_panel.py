from PyQt5.QtWidgets import QWidget, QPushButton, QHBoxLayout, QComboBox
from PyQt5.QtCore import Qt, QPoint
from PyQt5.QtGui import QColor

class FloatingButtonPanel(QWidget):
    """
    A floating panel that contains buttons and a drop-down menu.
    The drop-down menu changes the color of the first cell in the current column.
    """
    def __init__(self, parent, model, *args, **kwargs):
        super().__init__(parent, *args, **kwargs)
        self.model = model  # Store reference to the table model
        self.setAttribute(Qt.WA_StyledBackground, True)
        self.setWindowFlags(Qt.SubWindow)
        self.setStyleSheet("background-color: lightgray; border: 1px solid black;")

        self._drag_start_position = None
        self._init_ui()

    def _init_ui(self):
        # Create buttons (can be extended for additional functionality)
        self.button1 = QPushButton("Button 1", self)
        self.button2 = QPushButton("Button 2", self)

        # Create a drop-down menu
        self.dropdown = QComboBox(self)
        self.dropdown.addItem("conditions", QColor(Qt.white))
        self.dropdown.addItem("tags", QColor(Qt.cyan))
        self.dropdown.addItem("name", QColor(Qt.yellow))
        self.dropdown.currentIndexChanged.connect(self.change_column_color)

        # Layout with padding
        layout = QHBoxLayout(self)
        padding = 40
        layout.setContentsMargins(padding, padding, padding, padding)
        layout.setSpacing(5)
        layout.addWidget(self.button1)
        layout.addWidget(self.button2)
        layout.addWidget(self.dropdown)
        self.setLayout(layout)
        self.adjustSize()

    def change_column_color(self):
        """Change the background color of the first cell in the current column."""
        table = self.parent()
        if not table or not table.selectionModel():
            return

        selected_indexes = table.selectionModel().selectedIndexes()
        if not selected_indexes:
            return

        # Get the column of the first selected cell
        current_index = selected_indexes[0]  # Get the first selected index
        current_column = current_index.column()  # Column of the active selected cell
        selected_color = self.dropdown.currentData()

        # Update the model (this triggers persistence)
        self.model.set_column_color(current_column, selected_color)

    def mousePressEvent(self, event):
        if event.button() == Qt.LeftButton:
            self._drag_start_position = event.pos()
        super().mousePressEvent(event)

    def mouseMoveEvent(self, event):
        if event.buttons() & Qt.LeftButton and self._drag_start_position:
            new_pos = self.pos() + (event.pos() - self._drag_start_position)
            parent = self.parent()
            if parent:
                parent_width = parent.width()
                parent_height = parent.height()
                vertical_sb = parent.verticalScrollBar()
                horizontal_sb = parent.horizontalScrollBar()
                vertical_width = vertical_sb.width() if vertical_sb and vertical_sb.isVisible() else 0
                horizontal_height = horizontal_sb.height() if horizontal_sb and horizontal_sb.isVisible() else 0
                max_x = parent_width - vertical_width - self.width()
                max_y = parent_height - horizontal_height - self.height()
                new_x = max(0, min(new_pos.x(), max_x))
                new_y = max(0, min(new_pos.y(), max_y))
                new_pos = QPoint(new_x, new_y)
            self.move(new_pos)
        super().mouseMoveEvent(event)

    def mouseReleaseEvent(self, event):
        self._drag_start_position = None
        super().mouseReleaseEvent(event)
