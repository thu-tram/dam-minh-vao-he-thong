import os
import re

def reflow_markdown(file_path: str):
    with open(file_path, "r", encoding="utf-8") as f:
        lines = f.readlines()

    new_lines = []
    paragraph = []
    in_code_fence = False
    in_table = False

    def flush_paragraph():
        nonlocal paragraph
        if paragraph:
            new_lines.append(" ".join(paragraph) + "\n")
            paragraph = []

    for line in lines:
        stripped = line.rstrip("\n")

        # Toggle fenced code block (``` ...)
        if stripped.strip().startswith("```"):
            flush_paragraph()
            new_lines.append(line)
            in_code_fence = not in_code_fence
            continue

        if in_code_fence:
            new_lines.append(line)
            continue

        # Indented code block (4 spaces or tab at đầu dòng)
        if re.match(r"^( {4}|\t)", line):
            flush_paragraph()
            new_lines.append(line)
            continue

        # Phát hiện bảng (cứ thấy '|' thì giữ nguyên)
        if "|" or "+" in stripped:
            flush_paragraph()
            new_lines.append(line)
            in_table = True
            continue
        else:
            if in_table and stripped.strip() == "":
                in_table = False
            if in_table:
                new_lines.append(line)
                continue

        # Nếu dòng trống → kết thúc đoạn
        if stripped.strip() == "":
            flush_paragraph()
            new_lines.append("\n")
            continue

        # Nếu là heading, list item, blockquote → giữ nguyên
        if (
            re.match(r"^#{1,6}\s", stripped)    # heading
            or re.match(r"^>\s", stripped)      # blockquote
            or re.match(r"^(\d+\.\s)", stripped) # ordered list
            or re.match(r"^[-*]\s", stripped)   # unordered list
        ):
            flush_paragraph()
            new_lines.append(line)
            continue

        # Còn lại → gom vào paragraph
        paragraph.append(stripped.strip())

    # flush đoạn cuối cùng nếu còn
    flush_paragraph()

    with open(file_path, "w", encoding="utf-8") as f:
        f.writelines(new_lines)

    print(f"✔ Reflowed: {file_path}")


def reflow_markdown_dir(root_dir: str):
    for dirpath, _, filenames in os.walk(root_dir):
        for filename in filenames:
            if filename.endswith(".md"):
                reflow_markdown(os.path.join(dirpath, filename))


if __name__ == "__main__":
    # chạy cho toàn bộ thư mục hiện tại
    reflow_markdown_dir(".")
