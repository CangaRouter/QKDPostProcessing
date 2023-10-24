import json
import os
import sys
from typing import List


def recover_Data():
    qbers=list()
    times=list()
    # Iterate directory
    for path in os.listdir("."):
        # check if current path is a file
        if os.path.isfile(os.path.join(".", path)):
            if ".json" in path:
                filename = path
        file = open(path + "/" + filename, "r")
        data = json.load(file)
        qbers.push(double(data["Qber"]))
        times.push(double(data["Elsapsed_Real_Time"]))
    return qbers, times


x,y=recover_Data()

# Create a line graph
plt.plot(x, y)

# Add labels to the axes
plt.xlabel("Qber")
plt.ylabel("Elapsed_Real_Time")


# Display the graph
plt.show()
