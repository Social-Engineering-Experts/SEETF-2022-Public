#!/bin/bash

docker-compose -p ethereum up -d

cd DuperSuperSafeSafe
docker-compose -p ethereum-dsss up -d

cd ../YouOnlyHaveOneChance
docker-compose -p ethereum-yohoc up -d

cd ../Bonjour
docker-compose -p ethereum-bonjour up -d

cd ../RollsRoyce
docker-compose -p ethereum-rr up -d