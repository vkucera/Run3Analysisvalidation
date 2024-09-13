#!/usr/bin/env python3

"""
Script for making a table with derived HF datasets
Get the input JSON file at: https://alimonitor.cern.ch/alihyperloop-data/public/getHFdatasets.jsp
Author: Vit Kucera <vit.kucera@cern.ch>
"""

import argparse
import json
import sys


def main():
    """
    Main function
    """
    parser = argparse.ArgumentParser()
    parser.add_argument("path_input", type=str, help="input JSON file")
    parser.add_argument("--output", "-o", type=str, dest="path_output", help="output CSV file", default="table.csv")

    args = parser.parse_args()

    path_input = args.path_input
    path_output = args.path_output

    collision_systems = ["p-p", "Pb-Pb", None]
    parent_dataset_production_types = ["DATA", "MC", "HY"]

    # Dataset properties and their titles in the table header
    fields = {
        "parent_dataset_production_type" : "parent_dataset_production_type",
        "parent_dataset_name" : "Parent dataset",
        "parent_dataset_link" : "Hyperloop link to parent dataset",
        "dataset_id" : "dataset_id",
        "name" : "Derived dataset",
        "collision_system" : "collision_system",
        "dataset_link" : "Hyperloop link to derived dataset",
        "production_train" : "Production train",
        "parent_dataset_description" : "parent_dataset_description",
        "content" : "Content of derived data",
        "jira" : "JIRA",
    }

    # Selected properties to print in the table
    fields_selected = [
        "name",
        "dataset_link",
        "production_train",
        "content",
        "parent_dataset_name",
        "parent_dataset_link",
        "jira",
    ]

    # Table header
    header = ";".join([fields[key] for key in fields_selected])

    # Dictionary with lists for all combinations of collision systems and production types
    ds_lists = {}
    for coll in collision_systems:
        ds_lists[coll] = {}
        for type in parent_dataset_production_types:
            ds_lists[coll][type] = []

    # Load JSON
    try:
        with open(path_input, "r") as j:
            input_json = json.load(j)
    except FileNotFoundError:
        print(f"Error: Input file {path_input} does not exist.")
        sys.exit(1)

    # Process datasets entries.
    for _, dataset in input_json.items():
        ds = {}
        # Load dataset properties.
        for key in fields:
            ds[key] = dataset[key]
        # Adjustments
        # Take only the first line of the description.
        ds["content"] = ds["content"].split(r"\n")[0].replace(";", ".").strip()
        # If the JIRA URL is not in the "jira" field, try to get it from the description.
        if ds["jira"] is None and ds["parent_dataset_description"]:
            words = ds["parent_dataset_description"].split()
            is_url = [w.startswith("http") for w in words]
            if any(is_url):
                ds["jira"] = words[is_url.index(True)].split(r"\n")[0].strip()
        # Format the table row.
        line = ""
        for key in fields_selected:
            line += f"{ds[key]};"
        line = line[:-1]
        # Add the line in the list.
        ds_lists[ds["collision_system"]][ds["parent_dataset_production_type"]].append(line)

    # Print out lists.
    lines_out = ""
    for coll in collision_systems:
        for type in parent_dataset_production_types:
            lines_out += f"\n{coll} {type}\n"
            lines_out += f"{header}\n"
            for line in ds_lists[coll][type]:
                lines_out += f"{line}\n"

    # Write the table in the output file.
    if path_output:
        try:
            with open(path_output, "w") as file_out:
                file_out.write(lines_out)
        except IOError:
            print(f"Failed to open file {path_output}")
    else:
        print(lines_out)


if __name__ == "__main__":
    main()
