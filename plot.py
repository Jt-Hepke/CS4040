import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("results.csv") #read in file

# k = n rows and k 100n rows like assignment
small = df[df["k"] == df["n"]]
large = df[df["k"] == 100 * df["n"]]

algos = [("insertion_ms", "Insertion Sort", "tab:red"),
         ("quicksort_ms", "Quicksort", "tab:blue"),
         ("counting_ms", "Counting Sort", "tab:green")]

for col, name, color in algos:
    plt.plot(small["n"], small[col], color=color, linestyle="-",
             marker="o", label=f"{name} (k=n)")
    plt.plot(large["n"], large[col], color=color, linestyle="--",
             marker="s", label=f"{name} (k=100n)")

plt.xscale("log")
plt.yscale("log")
plt.xlabel("Input size n (log scale)")
plt.ylabel("Median runtime in ms (log scale)")
plt.title("Sorting Algorithm Runtimes")
plt.legend()
plt.grid(True, which="both", alpha=0.3)
plt.savefig("plot.png", dpi=200)
plt.show()