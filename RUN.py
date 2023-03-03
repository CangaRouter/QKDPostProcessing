from docker import from_env
from compose.cli.command import get_project

# Load the Docker environment
client = from_env()

# Load the Docker Compose project from the current directory
project = get_project('.')

# Start the Docker Compose project
project.up()

# Stop the Docker Compose project
project.down()
