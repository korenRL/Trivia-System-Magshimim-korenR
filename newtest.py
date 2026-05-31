import socket
import time
import json

SERVER_IP = '127.0.0.1'
SERVER_PORT = 8826

LOGIN_CODE = '1' 
SIGNUP_CODE = '2'

def send_and_receive(sock, message_description, msg_code, payload_dict):
    print(f"\n[>>>] Testing: {message_description}")
    
    json_payload = json.dumps(payload_dict)
    print(f"Sending JSON: {json_payload}")
    
    json_bytes = json_payload.encode('utf-8')
    data_length = str(len(json_bytes))
    
    msg_code_encoded = msg_code.encode()
    print("msg code encoded: ", end=' ')
    print(msg_code_encoded)

    header = msg_code.encode() + data_length.encode()

    print(header)

    
    full_paylod = header + json_bytes
    print(full_paylod) 

    sock.sendall(header + json_bytes)
    
    try:
        response = sock.recv(1024)
        if not response:
            print("[!] Server disconnected or didn't respond.")
            return
            
        if len(response) >= 5:
            res_code = response[0]
            res_length = int.from_bytes(response[1:5], byteorder='little')
            res_json = response[5:].decode('utf-8')
            print(f"[<<<] Server response (Code: {res_code}): {res_json}")
        else:
            print(f"[<<<] Server raw response: {response}")
            
    except socket.timeout:
        print("[!] Timeout: The server didn't respond in time. Check for crashes in C++!")
    except Exception as e:
        print(f"[!] Error receiving data: {e}")
        
    time.sleep(1)

def main():
    tester_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        tester_socket.connect((SERVER_IP, SERVER_PORT))
        print("[-] Successfully connected to the server, starting test suite...\n")

        test_user = "KorenVidTest"
        test_pass = "Password123!"
        test_email = "koren@example.com"

        payload = {"username": test_user, "password": test_pass}
        send_and_receive(tester_socket, "1. Login without prior registration", LOGIN_CODE, payload)

        payload = {"username": test_user, "password": test_pass, "email": test_email}
        send_and_receive(tester_socket, "Setup: Valid system registration", SIGNUP_CODE, payload)

        payload = {"username": test_user, "password": "AnotherPassword456!", "email": "another@example.com"}
        send_and_receive(tester_socket, "2. Duplicate registration with the same username", SIGNUP_CODE, payload)

        payload = {"username": test_user, "password": test_pass}
        send_and_receive(tester_socket, "Setup: Valid login of registered user", LOGIN_CODE, payload)

        payload = {"username": test_user, "password": test_pass}
        send_and_receive(tester_socket, "3. Login for an already logged-in user", LOGIN_CODE, payload)

        payload = {"username": "", "password": test_pass, "email": "empty@example.com"}
        send_and_receive(tester_socket, "4a. Input validation: Empty username", SIGNUP_CODE, payload)
        
        payload = {"username": "Koren!@#", "password": test_pass, "email": "special@example.com"}
        send_and_receive(tester_socket, "4b. Input validation: Username with special characters", SIGNUP_CODE, payload)

        print("\n[-] All tests for the video completed!")

    except ConnectionRefusedError:
        print(f"[!] Server is unavailable. Is it running on {SERVER_IP}:{SERVER_PORT}?")
    except Exception as e:
        print(f"[!] An unexpected error occurred: {e}")
    finally:
        tester_socket.close()

if __name__ == "__main__":
    main()