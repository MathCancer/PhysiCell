import itertools
import argparse
import os
import copy
import xml.etree.ElementTree as ET

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Script to generate physicell settings with  combiantions of node activations")
    parser.add_argument('-t', '--template',
                        dest="template",
                        action="store",
                        required=True,
                        help="Template to use on creating xml files.")
    parser.add_argument('-o', '--output',
                        dest="output",
                        action="store",
                        required=True,
                        help="Folder to use to save generated xml files.")
    parser.add_argument('-n', '--nodes',
                        dest="nodes",
                        nargs="+",
                        action="store",
                        required=True,
                        help="Nodes to create pair of activations.")
    parser.add_argument('-c', '--combinations',
                        dest="combinations",
                        type=int,
                        action="store",
                        required=True,
                        help="Number of current nodes activated.")

    options = parser.parse_args()
    template_source = os.path.abspath(options.template)
    destination_folder = os.path.abspath(options.output)

    all_nodes = set(options.nodes)

    combination_nodes = itertools.combinations(all_nodes, options.combinations)

    xml_tree = ET.parse(template_source)

    for combination in combination_nodes:
        custom_attributes = []
        for node in combination:
            element = ET.Element(node+"_node")
            element.text = '1'
            custom_attributes.append(element)
        
        remaining_nodes = all_nodes - set(combination)
        for node in remaining_nodes:
            element = ET.Element(node+"_node")
            element.text = '0'
            custom_attributes.append(element)

        new_xml = copy.deepcopy(xml_tree)
        root = new_xml.getroot()
        root.find("save").find("folder").text = "output_"+combination[0]+"_"+combination[1]
        root.find("user_parameters").extend(custom_attributes)
        
        new_xml.write(destination_folder+"/PhysiCell_settings_"+combination[0]+"_"+combination[1]+".xml")
