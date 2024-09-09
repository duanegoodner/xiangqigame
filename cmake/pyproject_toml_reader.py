import toml
import sys
from pathlib import Path


def parse_pyproject():
    try:
        # Load the pyproject.toml file
        with (Path(__file__).parent / "pyproject.toml").open(mode="r") as file:
            data = toml.load(file)

        # Extract information from the file
        name = data.get("project", {}).get("name", "")
        version = data.get("project", {}).get("version", "")
        description = data.get("project", {}).get("description", "")

        # You can customize the output based on what you need in CMake
        print(f"name={name}")
        print(f"version={version}")
        print(f"description={description}")

    except Exception as e:
        print(f"Error parsing pyproject.toml: {e}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    parse_pyproject()
