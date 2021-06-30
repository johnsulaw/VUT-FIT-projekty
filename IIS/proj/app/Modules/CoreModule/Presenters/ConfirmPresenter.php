<?php

namespace App\Modules\CoreModule\Presenters;

use Contributte\FormsBootstrap\BootstrapForm;
use Nette;
use Nette\Forms\Controls\BaseControl;

final class ConfirmPresenter extends Nette\Application\UI\Presenter
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

    // Show information about reservation
    public function renderDefault($hotel_id, $floor_room, $start, $end, $price) : void
    {
        $this->template->hotel = $this->database->table('hotel')->where('id', $hotel_id)->fetch();
        $this->template->room  = $floor_room;
        $this->template->start = date("Y M d",date_create($start)->getTimestamp());
        $this->template->end   = date("Y M d",date_create($end)->getTimestamp());
        $this->price = $price * (date_diff(date_create($start), date_create($end))->days + 1);
        $this->template->price = $this->price;
    }

    // Confirm order form
    public function createComponentConfirmForm()
    {
        $form = new BootstrapForm;

        // If not logged in, require personal information, to be identifiable by receptionist
        if (!$this->user->isLoggedIn()) {
            $form->addText('email')
                ->setHtmlAttribute('class','form-control')
                ->setHtmlAttribute('placeholder','Email')
                ->setRequired()
                ->addRule($form::EMAIL)
                ->addRule(function (BaseControl $input) { // User already exists
                    $mail = $input->getValue();
                    if ($this->database->table('user')->where('guest_mail', $mail)->fetch())
                        return false;
                    return true;
                }, 'There is already account registered on this mail')
                ->addRule(function (BaseControl $input) { // You can make up to 5 reservations
                    $mail = $input->getValue();
                    if ($this->database->table('reservation')->where('guest_mail', $mail)->count() >= 5)
                        return false;
                    return true;
                }, 'There are already 5 reservations made on this mail');

            $form->addText('first_name')->setRequired()
                ->setHtmlAttribute('class','form-control')
                ->setHtmlAttribute('placeholder','First name');
              
            $form->addText('last_name')->setRequired()
                ->setHtmlAttribute('class','form-control')
                ->setHtmlAttribute('placeholder','Last name');

            $form->addDate('birth_date','Date of birth')->setRequired()
                //->addRule(function(BaseControl $input) { // TODO validacia datumu
                 //   if ($input->value > new DateTime())
                //    {
                 //       return true;
               //     }
               //     return false;
                  //  }, 'Zadali ste nevalidný dátum')
                ->setHtmlAttribute('class','form-control');

            $form->addText('city')->setRequired()
                ->setHtmlAttribute('class','form-control')
                ->setHtmlAttribute('placeholder','City');

            $form->addText('street')->setRequired()
                ->setHtmlAttribute('class','form-control')
                ->setHtmlAttribute('placeholder','Street');

            // New account can be created, in that case, password is required
            $form->addCheckbox("create_account", "Create account?");
            $form->addPassword('password')
                ->setHtmlAttribute('class','form-control')
                ->setHtmlAttribute('placeholder','Password')
                ->addConditionOn($form['create_account'], $form::EQUAL, true)->setRequired()
                ->addRule($form::MIN_LENGTH, 'Password has to be at least 8 characters long',8);
        }

        $form->addSubmit("confirm", "Confirm")
            ->setHtmlAttribute("class", "btn btn-info");
        $form->onSuccess[] = [$this,'formSubmit'];

        return $form;
    }

    public function formSubmit(BootstrapForm $form)
    {
        $data = $form->getValues(); // Submitted data

        // User is not logged in
        if (!$this->user->isLoggedIn()) {
            // User is not in guests and mail is unique
            if (!$this->database->table('guest')->where('mail_address', $data->email)->fetch()) {
                // New guest
                $this->database->table('guest')->insert([
                    'mail_address' => $data->email,
                    'first_name'   => $data->first_name,
                    'last_name'    => $data->last_name,
                    'birth_date'   => $data->birth_date,
                    'city'         => $data->city,
                    'street'       => $data->street
                ]);
            }

            // New account creation, with role 3 => regular user
            if ($data->create_account) {
                $this->database->table('user')->insert([
                    'guest_mail' => $data->email,
                    'password'   => $data->password,
                    'role'       => 4
                ]);
                $user_row = $this->database->table('user')->where('mail_address', $data->email);
            }
        }

        // New reservation
        $this->database->table('reservation')->insert([
            'from_date'  => $this->start,
            'to_date'    => $this->end,
            'price'      => $this->price * (date_diff(date_create($this->start), date_create($this->end))->days + 1),
            'room_id'    => $this->floor_room,
            'hotel_id'   => $this->hotel_id,
            'guest_mail' => $this->user->isLoggedIn() ? $this->user->getId() : $data->email // TODO ??????
        ]);

        // Thank for using service and summary
        $this->redirect('Thank:', [
            'from_date' => $this->start,
            'to_date'   => $this->end,
            'price'     => $this->price,
            'room_id'   => $this->floor_room,
            'hotel_id'  => $this->hotel_id,
        ]);
    }
}
