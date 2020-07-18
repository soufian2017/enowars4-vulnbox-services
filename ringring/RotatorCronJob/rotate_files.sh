#!/usr/bin/env bash

mv /InvoiceApp/accounting/outstanding-invoices.log /InvoiceApp/accounting/outstanding-invoices.log.$(date +%d-%m-%Y-%H-%M-%S)
mv /InvoiceApp/accounting/settled-invoices.log /InvoiceApp/accounting/settled-invoices.log.$(date +%d-%m-%Y-%H-%M-%S)
echo "Files rotated."
