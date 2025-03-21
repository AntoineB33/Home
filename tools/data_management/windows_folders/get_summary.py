import os
import ast
from pathlib import Path

EXCLUDE_DIRS = {".git", "__pycache__", "venv", "env"}  # Add other directories to exclude

class FunctionVisitor(ast.NodeVisitor):
    def __init__(self):
        self.functions = []
        self.classes = []
        self.current_class = None

    def visit_ClassDef(self, node):
        class_name = node.name
        methods = []
        self.current_class = class_name
        for child in node.body:
            if isinstance(child, ast.FunctionDef):
                method_name = child.name
                args = self._get_args(child.args)
                returns = self._get_return_type(child.returns)
                methods.append(f"def {method_name}({args}){returns}")
        self.classes.append((class_name, methods))
        self.generic_visit(node)
        self.current_class = None

    def visit_FunctionDef(self, node):
        if not self.current_class:
            args = self._get_args(node.args)
            returns = self._get_return_type(node.returns)
            self.functions.append(f"def {node.name}({args}){returns}")
        self.generic_visit(node)

    def _get_args(self, args_node):
        args = []
        for arg in args_node.args:
            arg_str = arg.arg
            if arg.annotation:
                arg_str += f": {ast.unparse(arg.annotation)}"
            if hasattr(arg, 'default') and arg.default:
                default = ast.unparse(arg.default)
                arg_str += f" = {default}"
            args.append(arg_str)
        return ", ".join(args)

    def _get_return_type(self, returns_node):
        if returns_node:
            return f" -> {ast.unparse(returns_node)}"
        return ""

def generate_file_hierarchy(root_dir, prefix=""):
    lines = []
    try:
        entries = sorted(os.listdir(root_dir))
    except PermissionError:
        return lines  # Skip directories without permission
    
    dirs, files = [], []
    for e in entries:
        full_path = os.path.join(root_dir, e)
        if os.path.isdir(full_path):
            if e in EXCLUDE_DIRS:
                continue  # Skip excluded directories
            dirs.append(e)
        else:
            files.append(e)

    for d in dirs:
        lines.append(f"{prefix}|-- {d}")
        dir_path = os.path.join(root_dir, d)
        lines.extend(generate_file_hierarchy(dir_path, prefix + "    "))
    
    for f in files:
        lines.append(f"{prefix}|-- {f}")
    
    return lines

def parse_py_file(file_path):
    with open(file_path, "r", encoding="utf-8") as f:
        code = f.read()
    try:
        tree = ast.parse(code)
    except:
        return f"{file_path.name}: (Error parsing file)"
    visitor = FunctionVisitor()
    visitor.visit(tree)
    summary = [f"{file_path.name}:"]
    for class_name, methods in visitor.classes:
        summary.append(f"  class {class_name}:")
        for method in methods:
            summary.append(f"    {method}")
    for func in visitor.functions:
        summary.append(f"  {func}")
    return "\n".join(summary)

def parse_qml_file(file_path):
    with open(file_path, "r", encoding="utf-8") as f:
        lines = f.readlines()
    root_element, functions = None, []
    for line in lines:
        stripped = line.strip()
        if stripped.startswith("import"):
            continue
        if stripped and stripped[0].isupper() and not root_element:
            root_element = stripped.split("{")[0].strip()
            break
    for line in lines:
        stripped = line.strip()
        if stripped.startswith("function"):
            func_def = stripped[len("function") :].split("{")[0].strip()
            functions.append(f"function {func_def}")
    summary = [f"{file_path.name}:"]
    if root_element:
        summary.append(f"  Root element: {root_element}")
    if functions:
        summary.append("  Functions:")
        summary.extend([f"    {f}" for f in functions])
    return "\n".join(summary)

def main():
    project_root = Path.cwd()
    project_root = r"C:\Users\abarb\Documents\health\entertainment\news_underground\mediaSorter\programs\Python_C_solution"
    project_root = Path(project_root)
    output = ["File Hierarchy:\n"]

    # Generate file hierarchy
    hierarchy = generate_file_hierarchy(project_root)
    output.append("\n".join(hierarchy))

    # Generate code summaries
    output.append("\n\nCode Summaries:\n")
    for py_file in project_root.glob("**/*.py"):
        if "__pycache__" in str(py_file):
            continue
        output.append(parse_py_file(py_file))
    for qml_file in project_root.glob("**/*.qml"):
        output.append(parse_qml_file(qml_file))

    # Write to file
    with open(r"tools\data_management\windows_folders\project_summary.txt", "w", encoding="utf-8") as f:
        f.write("\n".join(output))

if __name__ == "__main__":
    main()