import socket
import time

SERVER_IP = '127.0.0.1'
SERVER_PORT = 5555

def send_and_receive(sock, message_description, message_data):
    print(f"\n[>>>] Testing: {message_description}")
    print(f"Sending data: {message_data}")
    
    sock.sendall(message_data.encode())
    
    try:
        response = sock.recv(1024).decode()
        print(f"[<<<] Server response: {response}")
    except Exception as e:
        print(f"[!] Error receiving data: {e}")
        
    time.sleep(0.5)

def main():
    tester_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        tester_socket.connect((SERVER_IP, SERVER_PORT))
        print("[-] Successfully connected to the server, starting test suite...\n")

        test_user = "KorenTestUser"
        test_pass = "Password123!"

        msg_login_unregistered = f"LOGIN|{test_user}|{test_pass}"
        send_and_receive(tester_socket, "Login without prior registration", msg_login_unregistered)

        msg_signup = f"SIGNUP|{test_user}|{test_pass}"
        send_and_receive(tester_socket, "Valid system registration", msg_signup)

        msg_signup_duplicate = f"SIGNUP|{test_user}|{test_pass}"
        send_and_receive(tester_socket, "Duplicate registration with the same username", msg_signup_duplicate)

        msg_login = f"LOGIN|{test_user}|{test_pass}"
        send_and_receive(tester_socket, "Valid login of a registered user", msg_login)

        msg_login_already_logged_in = f"LOGIN|{test_user}|{test_pass}"
        send_and_receive(tester_socket, "Additional login for an already logged-in user", msg_login_already_logged_in)

        msg_signup_empty_user = f"SIGNUP||{test_pass}"
        send_and_receive(tester_socket, "Input validation: Empty username", msg_signup_empty_user)
        
        msg_signup_special_chars = f"SIGNUP|User@Name|{test_pass}"
        send_and_receive(tester_socket, "Input validation: Username with special characters", msg_signup_special_chars)

        print("\n[-] All tests completed successfully!")

    except ConnectionRefusedError:
        print(f"[!] Server is unavailable. Is it running on {SERVER_IP}:{SERVER_PORT}?")
    except Exception as e:
        print(f"[!] An unexpected error occurred: {e}")
    finally:
        tester_socket.close()

if __name__ == "__main__":
    main()