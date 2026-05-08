import subprocess
import os
import re
import threading
import queue
import time

class EnigmaBackend:
    def __init__(self, exe_path):
        self.exe_path = exe_path
        self.process = None
        self.output_queue = queue.Queue()
        self.reader_thread = None
        self._start_process()

    def _start_process(self):
        if not os.path.exists(self.exe_path):
            raise FileNotFoundError(f"Исполняемый файл backend'а не найден: {self.exe_path}")
        
        backend_dir = os.path.dirname(self.exe_path)
        
        self.process = subprocess.Popen(
            [self.exe_path],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            bufsize=1,
            cwd=backend_dir
        )
        
        self.reader_thread = threading.Thread(target=self._read_output_loop, daemon=True)
        self.reader_thread.start()

    def _read_output_loop(self):
        try:
            for line in iter(self.process.stdout.readline, ''):
                if line:
                    self.output_queue.put(line)
        except Exception:
            pass

    def _send_input(self, text):
        if self.process and self.process.stdin:
            try:
                self.process.stdin.write(text + "\n")
                self.process.stdin.flush()
            except BrokenPipeError:
                print("Процесс завершился аварийно.")

    def _wait_for_prompt(self, stop_phrases, timeout=10.0):
        collected_lines = []
        start_time = time.time()
        
        while time.time() - start_time < timeout:
            try:
                line = self.output_queue.get(timeout=0.1)
                collected_lines.append(line)
                
                for phrase in stop_phrases:
                    if phrase in line:
                        return "".join(collected_lines)
                        
            except queue.Empty:
                continue
        
        return "".join(collected_lines) #+ "\n[Перезагрузка]"

    def _run_sequence(self, inputs, success_pattern):
        for inp in inputs:
            self._send_input(inp)
        output = self._wait_for_prompt(["Для продолжения процесса шифрования нажмите"])
        match = re.search(success_pattern, output)
        return match.group(1).strip() if match else output.strip()
    
    def encrypt(self, message, rotors, rings, stecker_active=False, stecker_pairs=""):
        r1, r2, r3 = rotors
        inputs = [
            "1",  #пункт 1 из меню
            "E",  #шифрование
            str(r1), str(r2), str(r3), #роторы
            rings,
            "Y" if stecker_active 
            else "N",
        ]
        if stecker_active:
            inputs.append(stecker_pairs.replace(" ", ""))
        inputs.append(message)
        inputs.append("N")
        
        return self._run_sequence(inputs, r"Your ciphered message is:\s*(.+)")

    def decrypt(self, ciphertext, rotors, rings, stecker_active=False, stecker_pairs=""):
        r1, r2, r3 = rotors
        inputs = [
            "1",
            "D",
            str(r1), str(r2), str(r3),
            rings,
            "Y" if stecker_active else "N",
        ]
        if stecker_active:
            inputs.append(stecker_pairs.replace(" ", ""))
        inputs.append(ciphertext)
        inputs.append("N")
        
        return self._run_sequence(inputs, r"Your decrypted message is:\s*(.+)")

    def get_info_file(self, filename):
        base_dir = os.path.dirname(self.exe_path)
        project_root = os.path.dirname(base_dir)
        filepath = os.path.join(project_root, "data", filename)
        
        try:
            with open(filepath, 'r', encoding='utf-8') as f:
                return f.read()
        except FileNotFoundError:
            return f"File '{filename}' не найдено в папке 'data'.\nПожалуйста, убедитесь, что файлы существуют."
        
    def get_history(self):
        backend_dir = os.path.dirname(self.exe_path)
        filepath = os.path.join(backend_dir, "Cipher_History.txt")
        
        try:
            with open(filepath, 'r', encoding='utf-8') as f:
                content = f.read()
            if not content.strip():
                return "История операций пуста."
            return content
        except FileNotFoundError:
            return "Файл истории не найден. Выполните первую операцию шифрования для его создания."  

    def run_crypto_analysis(self, sub_choice, text1, text2=""):
        inputs = [
            "5", #menu: cryptoanalysis
            str(sub_choice),
            text1,
            text2,
            ""  #press 'enter' to return
        ]
        
        for inp in inputs:
            self._send_input(inp)
            
        output = self._wait_for_prompt(["Press 'Enter' to go back to the main menu..."])
        
        lines = output.split('\n')
        filtered_lines = []
        
        skip_phrases = [
            "Cryptoanalysis Module",
            "Frequency Analysis",
            "Frequency analysis",
            "Check No-Self-Mapping",
            "Find Crib Positions",
            #"Enter choice",
            #"Enter ciphertext",
            #"Enter plaintext",
            "Enter crib",
            "-------"
        ]

        for line in lines:
            line = line.strip()
            if not line:
                continue
    
            if any(phrase in line for phrase in skip_phrases):
                continue
            
            if line[0:2] in ["1.", "2.", "3.", "4.", "5.", "6."]:
                continue
                
            if "Enigma Options" in line or "Ciphering" in line or "Exit" in line:
                continue
                
            if "Press 'Enter'" in line:
                continue
            
            filtered_lines.append(line)

        return '\n'.join(filtered_lines) if filtered_lines else "Backend error"
    
    def close(self):
        if self.process:
            try:
                self._send_input("6") #exit
                self.process.stdin.close()
                self.process.wait(timeout=2)
            except:
                self.process.terminate()