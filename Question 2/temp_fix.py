import re

def fix_includes(filepath, is_main):
    with open(filepath, 'r', encoding='utf-8') as f:
        content = f.read()
    
    # Remove all local `#include "..."`
    new_content = re.sub(r'(?m)^#include\s+"[^"]+".*\n?', '', content)
    
    if is_main:
        new_content = '#include "Q2_Submission.h"\n' + new_content
        
    with open(filepath, 'w', encoding='utf-8') as f:
        f.write(new_content)

fix_includes('Q2_Submission.h', False)
fix_includes('Q2_Main.cpp', True)
print("Includes fixed successfully.")
