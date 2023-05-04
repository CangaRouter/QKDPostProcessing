import matplotlib.pyplot as plt

def print_graph(x,y,xlabel,ylabel):
    # Create a line graph
    plt.plot(x, y)

    # Add labels to the axes
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)


    # Display the graph
    plt.show()
