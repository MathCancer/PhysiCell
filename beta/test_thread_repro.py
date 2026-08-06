import shutil
import subprocess
import sys
import tempfile
from datetime import datetime
import xml.etree.ElementTree as ET
from pathlib import Path


def update_config(xml_file, output_dir, threads, max_time, rng_mode):
    tree = ET.parse(xml_file)
    root = tree.getroot()

    root.find(".//overall/max_time").text = str(max_time)
    root.find(".//parallel/omp_num_threads").text = str(threads)

    save_folder = root.find(".//save/folder")
    if save_folder is None:
        save_node = root.find(".//save")
        if save_node is None:
            raise RuntimeError("Could not find <save> in config file")
        save_folder = ET.SubElement(save_node, "folder")
    save_folder.text = str(output_dir)

    random_seed = root.find(".//options/random_seed")
    if random_seed is None:
        random_seed = root.find(".//user_parameters/random_seed")
    if random_seed is not None:
        random_seed.text = "17"

    options = root.find(".//options")
    if options is None:
        options = ET.SubElement(root, "options")

    rng_mode_node = options.find("rng_mode")
    if rng_mode is None:
        if rng_mode_node is not None:
            options.remove(rng_mode_node)
    else:
        if rng_mode_node is None:
            rng_mode_node = ET.SubElement(options, "rng_mode")
        rng_mode_node.text = rng_mode

    tree.write(xml_file)


def run_once(repo_root, executable, config_file):
    cmd = [str(repo_root / executable), str(config_file)]
    subprocess.run(cmd, cwd=repo_root, check=True)


def compare_svg_outputs(repo_root, output_dir_a, output_dir_b):
    checker = repo_root / "beta" / "test_diff_svg.py"
    cmd = [sys.executable, str(checker), str(output_dir_a), str(output_dir_b)]
    subprocess.run(cmd, cwd=repo_root, check=True)


def main(executable, config_file, max_time, threads_a, threads_b, rng_mode="counter_based", work_dir=None):
    repo_root = Path.cwd()
    config_source = repo_root / config_file
    max_time = int(max_time)
    threads_a = int(threads_a)
    threads_b = int(threads_b)
    rng_mode_arg = rng_mode.strip().lower() if isinstance(rng_mode, str) else rng_mode
    if rng_mode_arg in ("", "off", "none", "omit", "omitted", "default"):
        rng_mode = None
    if threads_a == threads_b:
        raise SystemExit("threads_a and threads_b must be different")

    if work_dir is None:
        tmp_context = tempfile.TemporaryDirectory(prefix="physicell-thread-repro-")
        tmp_root = Path(tmp_context.__enter__())
        cleanup_context = tmp_context
    else:
        work_root = Path(work_dir)
        work_root.mkdir(parents=True, exist_ok=True)
        tmp_root = work_root / f"physicell-thread-repro_{datetime.now().strftime('%Y%m%d_%H%M%S')}"
        tmp_root.mkdir(parents=True, exist_ok=True)
        cleanup_context = None

    try:
        configs = []
        outputs = []

        for run_index, threads in enumerate((threads_a, threads_b), start=1):
            run_dir = tmp_root / f"run_{run_index}_threads_{threads}"
            output_dir = run_dir / f"output_threads_{threads}"
            output_dir.mkdir(parents=True, exist_ok=True)

            config_copy = run_dir / "PhysiCell_settings.xml"
            shutil.copy(config_source, config_copy)
            update_config(config_copy, output_dir, threads, max_time, rng_mode)

            print(
                f"\n\n {'=' * 80}\n running {executable} with {threads} threads "
                f"(rng_mode={rng_mode if rng_mode is not None else 'omitted'})"
            )
            print(f" config: {config_copy}")
            print(f" output: {output_dir}")
            run_once(repo_root, executable, config_copy)

            configs.append(config_copy)
            outputs.append(output_dir / "final.xml")

        for output in outputs:
            if not output.exists():
                raise SystemExit(f"Expected output file not found: {output}")

        compare_svg_outputs(repo_root, outputs[0].parent, outputs[1].parent)

        print(f"\n\n {'=' * 80}\n thread reproducibility test passed")
    finally:
        if cleanup_context is not None:
            cleanup_context.__exit__(None, None, None)


if __name__ == "__main__":
    main(*sys.argv[1:])