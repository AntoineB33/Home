# models/infinite_table_model.py

from PyQt5.QtCore import Qt, QAbstractTableModel, QModelIndex, QVariant
from PyQt5.QtGui import QColor

class InfiniteTableModel(QAbstractTableModel):
    """
    A QAbstractTableModel simulating a huge grid, backed by a TableStorage object.
    """
    def __init__(self, storage, rows=1000000, columns=1000000, parent=None):
        super().__init__(parent)
        self._row_count = rows
        self._col_count = columns
        self._storage = storage
        self._column_colors = {}  # Store column background colors

    def rowCount(self, parent=QModelIndex()):
        return self._row_count

    def columnCount(self, parent=QModelIndex()):
        return self._col_count

    def data(self, index, role=Qt.DisplayRole):
        if not index.isValid():
            return QVariant()

        row, col = index.row(), index.column()

        if role == Qt.DisplayRole or role == Qt.EditRole:
            return self._storage.get_value(row, col)

        if role == Qt.BackgroundRole and row == 0:  # Apply color only to first row
            return QColor(self._column_colors.get(col, Qt.white))

        return QVariant()

    def setData(self, index, value, role=Qt.EditRole):
        if index.isValid() and role == Qt.EditRole:
            self._storage.set_value(index.row(), index.column(), value)
            self.dataChanged.emit(index, index, [Qt.DisplayRole, Qt.EditRole])
            return True
        return False
    
    def set_column_color(self, col, color):
        """Set the background color of the first cell in the given column."""
        self._column_colors[col] = color
        index = self.index(0, col)  # First row, specified column
        self.dataChanged.emit(index, index, [Qt.BackgroundRole])  # Notify view about the color change


    def flags(self, index):
        if not index.isValid():
            return Qt.NoItemFlags
        # Make every cell editable and selectable.
        return Qt.ItemIsEditable | Qt.ItemIsSelectable | Qt.ItemIsEnabled
