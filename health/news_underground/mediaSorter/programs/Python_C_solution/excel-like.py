#!/usr/bin/env python3

import sys
from PyQt5.QtCore import Qt, QAbstractTableModel, QModelIndex, QVariant
from PyQt5.QtWidgets import QApplication, QTableView

class InfiniteTableModel(QAbstractTableModel):
    def __init__(self, rows=1000000, columns=1000000, parent=None):
        super().__init__(parent)
        self._row_count = rows
        self._col_count = columns
        # Dictionary to store data: keys are (row, col), values are the cell contents
        self._data = {}

    def rowCount(self, parent=QModelIndex()):
        # Return a large number to simulate "infinite" rows
        return self._row_count

    def columnCount(self, parent=QModelIndex()):
        # Return a large number to simulate "infinite" columns
        return self._col_count

    def data(self, index, role=Qt.DisplayRole):
        # DisplayRole is for showing the data in the table cells
        if not index.isValid() or role not in (Qt.DisplayRole, Qt.EditRole):
            return QVariant()

        row = index.row()
        col = index.column()

        # Return the cell data if present, otherwise empty string
        return self._data.get((row, col), "")

    def setData(self, index, value, role=Qt.EditRole):
        if index.isValid() and role == Qt.EditRole:
            row = index.row()
            col = index.column()
            self._data[(row, col)] = value
            # Let the view know data has changed
            self.dataChanged.emit(index, index, [Qt.DisplayRole, Qt.EditRole])
            return True
        return False

    def flags(self, index):
        # Make cells editable, selectable, enabled
        if not index.isValid():
            return Qt.NoItemFlags
        return Qt.ItemIsEditable | Qt.ItemIsSelectable | Qt.ItemIsEnabled


def main():
    app = QApplication(sys.argv)
    view = QTableView()
    
    model = InfiniteTableModel()
    view.setModel(model)
    
    # Optionally, set some initial size or behaviors:
    view.setWindowTitle("Excel-Like Infinite Grid")
    view.resize(800, 600)
    view.show()
    
    sys.exit(app.exec_())


if __name__ == "__main__":
    main()
