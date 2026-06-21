"""Fixtures for testing"""

import signal
import socket
import subprocess
from time import sleep

import pytest


@pytest.fixture(scope="session")
def server():
    """Start a server as a setUp and killing process as a tearDown."""

    process = subprocess.Popen(["./server", "server"])

    sleep(0.5)

    yield process

    try:
        process.send_signal(signal.SIGINT)
        process.wait(timeout=3)
    except subprocess.TimeoutExpired:
        process.kill()
        process.wait()


@pytest.fixture(scope="session")
def sock():
    """Initialize and connect a client socket."""

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.connect(("127.0.0.1", 3000))

    yield sock

    sock.close()
