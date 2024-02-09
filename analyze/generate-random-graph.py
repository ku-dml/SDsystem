import argparse
import random

def main():
    print("Start generating random graph")

    parser = argparse.ArgumentParser(description="Shuffle graph")
    parser.add_argument("-n", "--node", help = "the number of nodes", type = int)
    parser.add_argument("-p", "--probability", help = "probability", type = float)
    parser.add_argument("-s", "--seed", help = "seed of random", type = int, default = 10)
    parser.add_argument("-o", "--output", help = "output file", type = str, default = "graph.grh")

    args = parser.parse_args()

    random.seed(args.seed)

    with open(args.output, 'w') as file:
        for u in range(args.node):
            for v in range(u+1, args.node):
                if random.random() <= args.probability:
                    file.write("{}\t1\t{}\n".format(u, v))
                    file.write("{}\t1\t{}\n".format(v, u))
    print("Finished!")

if __name__ == "__main__":
    main()