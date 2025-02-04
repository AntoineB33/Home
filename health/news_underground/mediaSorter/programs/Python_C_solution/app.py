from controllers.main_controller import MainController

def main():
    controller = MainController()
    exit_code = controller.run()
    return exit_code

if __name__ == "__main__":
    import sys
    sys.exit(main())
