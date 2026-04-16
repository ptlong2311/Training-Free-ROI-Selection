import requests
import json

def check_packaging(token, type, Barcode):
    body = {
        "command": {
            "Type": type,
            "Barcode": Barcode
        }
    }
    headers = {
        "Authorization": f"Bearer {token}"
    }
    resp = requests.post("http://192.168.1.3:5438/sit-svc/application/FBSFICLLApp/odata/CheckPackagingCmd", json=body, headers=headers)
    
    data = resp.json()
    if "MSGCODE" in data and "MSGTYPE" in data and "MSGINFOR" in data:
        if (
            data["MSGCODE"] == "1" and
            data["MSGTYPE"] == "S" and
            data["MSGINFOR"].strip() =="Data  saving is OK"
        ):
            return (True, data["MSGDATA"])
        else:
            return (False, "Data Incorrect")
    else:
        return (False, "Invalid Data Format")

if __name__ == "__main__":
    token = "ManualPacking011"
    result = check_packaging(token, 2, "rqw4fqfqfr")
    print(result)
