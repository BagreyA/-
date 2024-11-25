                                                                                                                                                                  API_KEY="5baf2e5468864362b1b122054242311"
BASE_URL="http://api.weatherapi.com/v1"

# Список метеостанций с индексами
declare -A stations
stations["Santa Fe"]=87501
stations["Washington"]=20001
stations["Mathews"]=36052
stations["Olympia"]=98501
stations["Richmond"]=23225
stations["New York"]=10023
stations["Honolulu"]=96802
stations["Atlanta"]=30304
stations["Sacramento"]=94205
stations["Topeka"]=66604
stations["Boston"]=02109
stations["Saint Paul"]=55103
stations["Lincoln"]=68502
stations["Trenton"]=08603
stations["Columbia"]=29202

# Функция для вывода списка станций с индексами
list_stations() {
    echo "Список метеостанций с индексами:"
    for city in "${!stations[@]}"; do
        echo "Город: $city, Индекс: ${stations[$city]}"
    done
}

# Функция для отображения погоды
display_weather() {
    local response="$1"
    local period="$2"
    case $period in
    1)
        echo "Текущая погода:"
        echo "$response" | jq -r '. | "Температура: \(.current.temp_c)°C, Ощущается как: \(.current.feelslike_c)°C, Влажность: \(.current.humidity)%, Давление: \(.current.pressure_mb) мбар, Ветер: \(.current.wind_dir) \(.current.wind_kph) км/ч, Описание: \(.current.condition.text)"'
        ;;
    2)
        echo "Прогноз на день:"
        echo "$response" | jq -r '.forecast.forecastday[0] as $f | [
            "Ночь \($f.date): Температура: \($f.hour[0].temp_c)°C, Давление: \($f.hour[0].pressure_mb) мбар, Ветер: \($f.hour[0].wind_dir) \($f.hour[0].wind_kph) км/ч",
            "Утро \($f.date): Температура: \($f.hour[6].temp_c)°C, Давление: \($f.hour[6].pressure_mb) мбар, Ветер: \($f.hour[6].wind_dir) \($f.hour[6].wind_kph) км/ч",
            "День \($f.date): Температура: \($f.hour[12].temp_c)°C, Давление: \($f.hour[12].pressure_mb) мбар, Ветер: \($f.hour[12].wind_dir) \($f.hour[12].wind_kph) км/ч",
            "Вечер \($f.date): Температура: \($f.hour[18].temp_c)°C, Давление: \($f.hour[18].pressure_mb) мбар, Ветер: \($f.hour[18].wind_dir) \($f.hour[18].wind_kph) км/ч"
            
         ] | join(", 
         ")'
    ;;
    3)
        echo "Прогноз на неделю:"
        echo "$response" | jq '.forecast.forecastday[] | {
            "Дата": .date,
            "Максимальная температура (°C)": .day.maxtemp_c,
            "Минимальная температура (°C)": .day.mintemp_c,
            "Средняя влажность (%)": .day.avghumidity,
            "Скорость ветра (км/ч)": .day.maxwind_kph,
            "Описание": .day.condition.text,
            "Время восхода": .astro.sunrise,
            "Время заката": .astro.sunset
        }'
        ;;
    esac
}

# Функция для получения погоды по названию города
get_weather_by_city() {
    read -p "Введите название города: " city
    echo "Выберите период прогноза:"
    echo "1. Погода на данный момент."
    echo "2. Погода на день."
    echo "3. Погода на неделю."
    read -p "Ваш выбор: " period

    case $period in
    1)
        response=$(curl -s "${BASE_URL}/current.json?key=${API_KEY}&q=${city}&lang=ru")
        ;;
    2)
        response=$(curl -s "${BASE_URL}/forecast.json?key=${API_KEY}&q=${city}&days=1&lang=ru")
        ;;
    3)
        response=$(curl -s "${BASE_URL}/forecast.json?key=${API_KEY}&q=${city}&days=7&lang=ru")
        ;;
    *)
        echo "Неверный выбор!"
        return
        ;;
    esac

    if echo "$response" | grep -q '"error"'; then
        echo "Ошибка: Город не найден или превышен лимит запросов!"
    else
        display_weather "$response" "$period"
    fi
}

# Функция для получения погоды по синоптическому индексу
get_weather_by_index() {
    read -p "Введите синоптический индекс: " index
    echo "Выберите период прогноза:"
    echo "1. Погода на данный момент."
    echo "2. Погода на день."
    echo "3. Погода на неделю."
    read -p "Ваш выбор: " period

    case $period in
    1)
        response=$(curl -s "${BASE_URL}/current.json?key=${API_KEY}&q=${index}&lang=ru")
        ;;
    2)
        response=$(curl -s "${BASE_URL}/forecast.json?key=${API_KEY}&q=${index}&days=1&lang=ru")
        ;;
    3)
        response=$(curl -s "${BASE_URL}/forecast.json?key=${API_KEY}&q=${index}&days=7&lang=ru")
        ;;
    *)
        echo "Неверный выбор!"
        return
        ;;
    esac

    if echo "$response" | grep -q '"error"'; then
        echo "Ошибка: Индекс не найден или превышен лимит запросов!"
    else
        city=$(echo "$response" | jq -r '.location.name')
        display_weather "$response" "$period"
    fi
}

# Меню
while true; do
    echo "Меню:"
    echo "1. Вывести список метеостанций с индексами."
    echo "2. Узнать погоду в городе."
    echo "3. Узнать погоду по синоптическому индексу."
    echo "4. Выйти."
    
    read -p "Выберите опцию: " option

    case $option in
    1)
        list_stations
        ;;
    2)
        get_weather_by_city
        ;;
    3)
        get_weather_by_index
        ;;
    4)
        echo "Выход из программы."
        break
        ;;
    *)
        echo "Неверный выбор! Попробуйте снова."
        ;;
    esac
done