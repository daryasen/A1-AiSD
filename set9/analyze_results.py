import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("all_results.csv", sep=";")

for array_type in df["ArrayType"].unique():
    data = df[df["ArrayType"] == array_type]

    #TIME GRAPH
    plt.figure(figsize=(12, 7))

    plt.plot(data["Size"], data["QuickTime"],
             marker="o", label="QuickSort")

    plt.plot(data["Size"], data["MergeTime"],
             marker="o", label="MergeSort")

    plt.plot(data["Size"], data["StringMergeLCPTime"],
             marker="o", label="String MergeSort LCP")

    plt.plot(data["Size"], data["TernaryQuickTime"],
             marker="o", label="Ternary QuickSort")

    plt.plot(data["Size"], data["MSDTime"],
             marker="o", label="MSD Radix Sort")

    plt.plot(data["Size"], data["MSDSwitchTime"],
             marker="o", label="MSD Radix + Switch")

    plt.title(f"Execution Time - {array_type}")
    plt.xlabel("Array Size")
    plt.ylabel("Time (ms)")
    plt.legend()
    plt.grid(True)

    plt.savefig(f"time_{array_type}.png")
    plt.close()

    #COMPARISONS GRAPH
    plt.figure(figsize=(12, 7))

    plt.plot(data["Size"], data["QuickComparisons"],
             marker="o", label="QuickSort")

    plt.plot(data["Size"], data["MergeComparisons"],
             marker="o", label="MergeSort")

    plt.plot(data["Size"], data["StringMergeLCPComparisons"],
             marker="o", label="String MergeSort LCP")

    plt.plot(data["Size"], data["TernaryQuickComparisons"],
             marker="o", label="Ternary QuickSort")

    plt.plot(data["Size"], data["MSDComparisons"],
             marker="o", label="MSD Radix Sort")

    plt.plot(data["Size"], data["MSDSwitchComparisons"],
             marker="o", label="MSD Radix + Switch")

    plt.title(f"Character Comparisons - {array_type}")
    plt.xlabel("Array Size")
    plt.ylabel("Character Comparisons")
    plt.legend()
    plt.grid(True)

    plt.savefig(f"comparisons_{array_type}.png")
    plt.close()

print("Графики успешно созданы!")