<?php


namespace App\Modules\CoreModule\Presenters;
use Nette;

class ThankPresenter extends Nette\Application\UI\Presenter
{
    /**
     * @inject
     * @var Nette\Database\Context
     */
    public $database;

    public function renderDefault($from_date, $to_date, $price, $room_id, $hotel_id) {
        $this->template->hotel = $this->database->table('hotel')
            ->where('id', $hotel_id)->fetch();
        $this->template->room = $this->database->table('room')
            ->where('floor_room', $room_id)->fetch();
        
        $this->template->days = (date_diff(date_create($from_date), date_create($to_date))->days + 1);
        $this->template->price = $price;
        $this->template->total_price = $price * (date_diff(date_create($from_date), date_create($to_date))->days + 1);
        $this->template->from = date("Y M d",date_create($from_date)->getTimestamp());
        $this->template->to = date("Y M d",date_create($to_date)->getTimestamp());
        $this->template->img = "../img/hotels/".$this->template->hotel->image;
    }
}