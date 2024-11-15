import requests
import argparse
from bs4 import BeautifulSoup

#
# Script to download RINEX files from https://gnss.osi.ie
# Note: please agree to terms on site before downloading.
# Joe Desbonnet 2024-11-15
#

def output_response_info (session, response) :

    #print(f"Request URL: {form_endpoint}")
    print("Request Headers:")
    print(response.request.headers)
    print("\nResponse Status Code:", response.status_code)
    print("Response Cookies:", session.cookies.get_dict())
    print("Response Headers:")
    print(response.headers)
    print("\n----")



def download_data(station_id, date, start, end):
    # Base URL for the website
    base_url = "https://gnss.osi.ie" 
    form_endpoint = f"{base_url}/" 
    download_endpoint = f"{base_url}/?download"
    
    # Start a session to maintain cookies
    session = requests.Session()
    
    # Step 1: Access the home page to initiate the session
    print("Initializing session...")
    response = session.get(form_endpoint)
    output_response_info (session,response)
    if response.status_code != 200:
        print(f"Failed to access the home page. Status code: {response.status_code}")
        return
    



    print("\nExtracting hidden form field 'as_sfid'...")
    soup = BeautifulSoup(response.text, 'html.parser')
    hidden_field = soup.find('input', {'name': 'as_sfid'})
    if not hidden_field or not hidden_field.get('value'):
        print("Error: 'as_sfid' hidden field not found in the form.")
        return
    as_sfid = hidden_field['value']
    print(f"Extracted 'as_sfid': {as_sfid}")



    # Step 2: Submit the form with the filter parameters
    print("Submitting filter form...")
    form_data = {
        'station0': station_id,
        'date': date,
        'start': start,
        'end':end,
        'submitSearchByStation': 'FIND DATA',
        'as_sfid': as_sfid
    }
    response = session.post(form_endpoint, data=form_data)
    output_response_info(session,response)
    if response.status_code != 200:
        print(f"Failed to submit the form. Status code: {response.status_code}")
        return
    
    # Step 3: Trigger the download
    print("Downloading data...")
    response = session.get(download_endpoint)
    if response.status_code == 200:
        filename = f"DATA_{station_id}_{date}_{start:02d}_{end:02d}.zip"
        with open(filename, 'wb') as file:
            file.write(response.content)
        print(f"Data successfully downloaded and saved as {filename}")
    else:
        print(f"Failed to download data. Status code: {response.status_code}")


if __name__ == '__main__':

    parser = argparse.ArgumentParser(description="Download GNSS RINEX files from https://gnss.osi.ie. Please agree to T&C on site first.")
    parser.add_argument("--station-id", help="Station ID. Example glw1 (Galway)", required=True)
    parser.add_argument("--date",help="Date of download yyyy-mm-dd", required=True)
    parser.add_argument("--start-hour",help="Hour (UTC) 0 to 22", default="0")
    parser.add_argument("--end-hour", help="End hour (UTC) 1 to 23", default="24")

    args = parser.parse_args()

    start = int(args.start_hour)
    if args.end_hour :
        end = int(args.end_hour)
    else :
        end = start + 1

    print (f"date={args.date} start={start} end={end}")

    download_data(args.station_id, args.date, start, end)


