<?php


namespace App\Modules\CoreModule\Presenters;
use Nette;

class ReservationPresenter extends Nette\Application\UI\Presenter
{
    /**
     * @inject
     * @var Nette\Database\Context
     */
    public $database;
    /** @persistent */
    public $hotel_id;
    /** @persistent */
    public $type;
    /** @persistent */
    public $start;
    /** @persistent */
    public $end;
    /** @persistent */
    public $floor_room;
    /** @persistent */
    public $price;

    // Render information about room
    public function renderDefault(int $hotel_id, string $type): void
    {
        $this->template->hotel = $this->database->table('hotel')->where('id', $hotel_id)->fetch();
        $this->template->room = $this->database->table('room_details')
            ->where(['type' => $type, 'hotel_id' => $hotel_id])->fetch();
        $this->template->type = $type;
        $this->template->dateMin = date("m/d/Y");
        $this->template->dateMax = date("m/d/Y", mktime(0,0,0,date("m") + 1));



        // load galleries
        $this->template->gallery = $this->template->hotel->gallery;
        $directory = "../www/img/galleries/".$this->template->gallery;
        $scanned_directory = array_diff(scandir($directory), array('..', '.'));
        $this->template->images = $scanned_directory;
    }

    // Date range was picked
    public function handleChangeDate() : void
    {
        // Check request
        if (!$this->isAjax() || !isset($_GET) || !isset($_GET['start']) || !isset($_GET['end'])) { exit(0); }

        $this->start = $_GET['start'];
        $this->end = $_GET['end'];

        // Check validity
        if ($this->start > $this->end) {
            echo "invalid date";
            return;
        }

        // Rooms that fit the criteria
        $valid_rooms =
            $this->database->table('room')->
            where(['type' => $this->type, 'hotel_id' => $this->hotel_id])->fetchAll();

        // Rooms in a hotel of certain type
        foreach ($valid_rooms as $v) {
            $reservations =
                $this->database->table('reservation')->
                where(['room_id' => $v['floor_room'], 'hotel_id' => $v['hotel_id']])->fetchAll();

            // Reservations, looking for conflicts
            $conflict = false;
            foreach ($reservations as $r) {
                // Reservation star or end is in time of different reservation
                if ($r['from_date'] < $this->start && $this->start < $r['to_date'] ||
                    $r['from_date'] < $this->end && $this->end < $r['to_date']) {
                    // All rooms had conflicts
                    $conflict = true;
                    break;
                }
            }

            // Reservation in this room can be made
            if (!$conflict) {
                $this->floor_room = $v['floor_room'];
                $this->redirect('Confirm:',
                    [
                        'hotel_id' => $this->hotel_id,
                        'floor_room' => $this->floor_room,
                        'start' => $this->start,
                        'end' => $this->end,
                        'price' => $this->database->table('room_details')
                            ->where(['type' => $this->type, 'hotel_id' => $this->hotel_id])->fetch()['price']
                    ]);
            }
        }

        echo "We are sorry, this hotel can not give you any room at this date.";
        exit(0);
    }
}