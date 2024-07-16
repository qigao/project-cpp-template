#!/bin/bash

# Set variables
SERVER_NAME="example.com"
CLIENT_NAME="Client"
CA_NAME="MyCA"
DAYS_VALID=36500

mkdir certs
cd certs
# Generate CA key and certificate
openssl genrsa -out rootCA.key 4096
openssl req -x509 -new -nodes -key rootCA.key -sha256 -days $DAYS_VALID -out rootCA.pem -subj "/CN=$CA_NAME"

# Generate server key and CSR
openssl genrsa -out $SERVER_NAME.key 2048
openssl req -new -key $SERVER_NAME.key -out $SERVER_NAME.csr -subj "/CN=$SERVER_NAME"

# Generate server certificate
openssl x509 -req -in $SERVER_NAME.csr -CA rootCA.pem -CAkey rootCA.key -CAcreateserial -out $SERVER_NAME.pem -days $DAYS_VALID -sha256


# Generate client key and CSR
openssl genrsa -out client.key 2048
openssl req -new -key client.key -out client.csr -subj "/CN=$CLIENT_NAME"

# Generate client certificate
openssl x509 -req -in client.csr -CA rootCA.pem -CAkey rootCA.key -CAcreateserial -out client.pem -days $DAYS_VALID -sha256

# Create client CA bundle
cat client.pem rootCA.pem > client-ca-bundle.pem

# Verify server certificate
echo "Verifying server certificate:"
openssl verify -CAfile rootCA.pem $SERVER_NAME.pem

# Verify client certificate
echo "Verifying client certificate:"
openssl verify -CAfile rootCA.pem client.pem

# Verify client CA bundle
echo "Verifying client CA bundle:"
openssl verify -CAfile rootCA.pem client-ca-bundle.pem

cd ..