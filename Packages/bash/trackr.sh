# How to use:
#
# trackr.sh add [field] [value]
# trackr.sh add-batch [field] [values...]
# trackr.sh get [field] [offset] [limit] [order]
#
# Set TRACKR_API_KEY environment variable to your API key. 
# You can override the default host by setting TRACKR_API_HOST environment variable

ApiEndpoint="http://wryneck.cs.umanitoba.ca/api/values"

function AddSingleValue() {
    local apiKey="$1"
    local fieldId="$2"
    local value="$3"

    if ! [[ "$apiKey" =~ ^[[:alnum:]]{64}$ ]]; then
        echo "Invalid apiKey. Please provide a valid 64-character alphanumeric string."
        exit 1
    fi

    local data="apiKey=$apiKey&value=$value&fieldId=$fieldId"
    response=$(curl -L -s -X POST "$ApiEndpoint" --header "Content-Type: application/x-www-form-urlencoded" --data "$data")
    
    echo $response
}

function AddManyValues() {
    local apiKey="$1"
    local fieldId="$2"
    local values=("${@:3}")

    if ! [[ "$apiKey" =~ ^[[:alnum:]]{64}$ ]]; then
        echo "Invalid apiKey. Please provide a valid 64-character alphanumeric string."
        exit 1
    fi

    for value in "${values[@]}"; do
        echo "Adding value: $value"
        response=$(AddSingleValue "$apiKey" "$fieldId" "$value")
        echo "Response: $response"
        sleep 1
    done
}

function GetValues() {
    local apiKey="$1"
    local fieldId="$2"
    local offset="$3"
    local limit="$4"
    local order="$5"

    if ! [[ "$apiKey" =~ ^[[:alnum:]]{64}$ ]]; then
        echo "Invalid apiKey. Please provide a valid 64-character alphanumeric string."
        exit 1
    fi

    if [[ "$order" != "asc" && "$order" != "desc" ]]; then
        echo "Order must be 'asc' or 'desc'. Please try again."
        exit 1
    fi

    local params="apiKey=$apiKey&fieldId=$fieldId&offset=$offset&limit=$limit&order=$order"
    response=$(curl -L -s -X GET "$ApiEndpoint?$params")
    echo "$response"
}

if [ -z "${TRACKR_API_KEY}" ]; then
    echo "API key is unset. Please set TRACKR_API_KEY environment variable"
    exit 0
fi
    
if [ -n "${TRACKR_API_HOST}" ]; then
    ApiEndpoint = "${TRACKR_API_HOST}"
fi


if [[ "$1" = "add" ]]; then
    AddSingleValue "${TRACKR_API_KEY}" "$2" "$3"
elif [[ "$1" = "add-batch" ]]; then
    AddManyValues "${TRACKR_API_KEY}" "$2" "${@:3}"
elif [[ "$1" = "get" ]]; then
    GetValues "${TRACKR_API_KEY}" "$2" "$3" "$4" "$5"
else
    echo "Unknown command"
fi