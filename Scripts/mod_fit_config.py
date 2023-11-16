import os
import re
from pathlib import Path
from datetime import datetime
import argparse
from utils.utils import check_inputpath, check_outputpath
import logging

def modify_config_files(configdir_path, histo_path, new_configdir_path, new_outputdir_path, new_debuglevel):
    dir_path = check_inputpath(configdir_path)
    histo_path = check_inputpath(histo_path)
    new_config_dirpath = check_outputpath(new_configdir_path)

    logging.basicConfig(level=logging.INFO)
    logging.info("\n " +
                 f"Modifying config files in {dir_path.absolute()} \n "+
                 f"with HistoPath: {histo_path.absolute()} \n "+
                 f"new ConfigDir: {new_config_dirpath.absolute()} \n "+
                 f"new OutputDir: {new_outputdir_path} \n " +
                 f"new DebugLevel: {new_debuglevel}")

    config_files = sorted(dir_path.rglob("*.config"))

    for file_path in config_files:
        with open(file_path, 'r') as file:
            content = file.read()

        config_file = file_path.name
        wp_mass_match = re.search(r'm(\d+)', config_file)
        wp_mass = wp_mass_match.group(1) if wp_mass_match else "UNKNOWN"

        content = re.sub(r'HistoPath: ".*?"', f'HistoPath: "{histo_path}"', content)
        # Now, add the new OutputDir line after the HistoPath line:
        newline_outputdir = f'  OutputDir: {new_outputdir_path} '
        content = re.sub(r'(HistoPath: ".*?")', r'\1\n' + newline_outputdir, content)

        content = re.sub(r'DebugLevel: \d', f'DebugLevel: {new_debuglevel}', content)

        # Pattern to identify the line with "Author"
        author_pattern = r"(# Author:.*\n)"

        # Check if the pattern is found in the content
        if re.search(author_pattern, content):
            # Replacement includes the found line (using \1) followed by the "Modified:" line
            author_replacement = r"\1# Modified by: Haoran Zhao        #\n"
            content = re.sub(author_pattern, author_replacement, content)


        # Pattern to identify the line with "Date"
        date_pattern = r"(# Date: )(\d+\.\d+\.\d+)(\s+#)"

        # Check if the date pattern is found in the content
        date_match = re.search(date_pattern, content)
        if date_match:
            # Get today's date and format it as "dd.mm.yyyy"
            today_date = datetime.now().strftime('%d.%m.%Y')
            # Calculate spaces required to maintain alignment
            space_count = len(re.search(date_pattern, content).group(2)) - len(today_date)
            
            # The replacement pattern keeps the prefix, replaces the date, and then adds the correct number of spaces to maintain alignment
            date_replacement = date_match.group(1) + today_date + " " * space_count + date_match.group(3)
                
            content = content.replace(date_match.group(0), date_replacement)

        with open(os.path.join(new_config_dirpath, config_file), 'w') as file:
            file.write(content)

        logging.info(f"Config file {config_file} updated successfully for mass point: {wp_mass}")

def main():
    parser = argparse.ArgumentParser(description="Modify config files with the given parameters.")
    parser.add_argument('configdir_path', type=str, help="Directory path containing the config files.")
    parser.add_argument('histo_path', type=str, help="New HistoPath value.")
    parser.add_argument('--new-configdir-path', type=str, help="New directory path to store modified config files.", default="./new_configdir_path")
    parser.add_argument('--new-outputdir-path', type=str, help="New OutputDir value.", default="/data/hrzhao/Samples/ssWWWZ_run3/MyOutputs/testarea/")
    parser.add_argument('--new-debuglevel', type=int, help="New DebugLevel value.", default=1, nargs='?')

    args = parser.parse_args()

    modify_config_files(args.configdir_path, args.histo_path, args.new_configdir_path, args.new_outputdir_path, args.new_debuglevel)

if __name__ == '__main__':
    main()
