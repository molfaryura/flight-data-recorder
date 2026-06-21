"""Tests for udp socket"""

from time import sleep


def test_start_server(server):
    """Test verifies that server successfully started."""

    assert server.poll() is None, "Server failed to start."


def test_one_message(sock):
    """Test verifies that message is successfully writtent to the file."""

    message = "$TELEMETRY,20,90,40#"

    sock.send(message.encode())

    sleep(1)

    with open("blackbox.bin", "rb") as file:
        content = file.read()

    assert b"TELEMETRY" in content, "TELEMETRY is not in file."
